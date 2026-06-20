# Rendering Pipeline

The project uses a CPU-side software 3D pipeline and SDL2 for final 2D drawing.
SDL supplies the window, renderer, lines, and filled scanlines; transformation,
lighting, clipping, projection, and visibility decisions are implemented by the
project.

The main entry point is `World::render()` in `src/world.cpp`. The final drawing
primitives are implemented in `src/graphics.cpp`.

## Coordinate Spaces

Geometry passes through these representations:

| Space | Storage | Description |
| --- | --- | --- |
| Local | `Master::vert` | Reusable mesh coordinates before master or instance transforms. |
| World | `Instance::world_vert` | Local vertices after master scale, instance scale, rotations, and translation. |
| Viewer/normalized | `Instance::view_vert` | World vertices translated and rotated relative to the viewer, then scaled for the perspective volume. |
| Screen | Temporary integer coordinates | Projected pixel positions relative to the screen centre. |

With a viewer direction of `(0, 0, 0)`, visible geometry lies along negative Z.

## Parse-Time Preparation

When the parser finishes an instance, it performs two operations that rendering
later relies upon:

1. `Instance::local_to_world()` creates `world_vert`.
2. `Instance::setup_bounds()` calculates the minimum and maximum world-space
   corners of its axis-aligned bounding box.

The local-to-world transform order is:

```text
local vertex
  -> master scale
  -> instance scale
  -> master rotation
  -> instance rotation
  -> instance translation
  -> world vertex
```

These values are cached. The current frame loop moves the viewer but does not
move instances, so it does not need to rebuild them every frame.

## Per-Frame Sequence

`main.cpp` calls `World::render()` once per frame. Rendering proceeds in this
order:

1. Clear the upper half of the renderer with the sky colour and the lower half
   with the ground colour.
2. Transform every instance from world space into viewer space.
3. Calculate a flat colour for every triangle.
4. Sort instances from far to near.
5. Clip, project, and collect primitives for each instance.
6. Sort that instance's projected primitives from far to near.
7. Rasterise the primitives through the SDL-backed 2D drawing functions.
8. Return to `main.cpp`, which draws the pointer and presents the renderer.

The rest of this document describes those stages in more detail.

## World To Viewer Transform

`Instance::world_to_viewer()` transforms each world vertex by:

1. Subtracting the viewer position.
2. Rotating by the negative viewer angle.
3. Scaling into the renderer's normalized perspective volume.

The normalization currently uses fixed view-reference and back-plane constants
of `-50` and `-75`. The resulting X, Y, and Z values are stored in `view_vert`.

The same transform is applied to the minimum and maximum corners of the
instance's world-space bounding box. The larger squared distance of those two
corners becomes `Instance::order`, which is used for instance sorting.

## Lighting

`Instance::setup_color()` calculates one colour per source triangle in world
space. It first derives a face normal from the triangle's transformed vertices,
so lighting is flat rather than interpolated across pixels.

Each face colour combines:

- Ambient light from the viewer's ambient colour and intensity.
- A global directional light stored on `Viewer`.
- A positional light owned by `World`.
- A specular term scaled by the instance's `specularity` percentage.

The result is stored in `Instance::poly_color` and reused by every clipped piece
of that source triangle. `Color` converts and clamps the final floating-point
channels when the draw colour is sent to SDL.

Lighting is recalculated every frame even though the current meshes and lights
are static, because the specular calculation depends on the viewer position.

## Painter-Style Instance Sorting

Instances are sorted by `Instance::order` in descending order, so the estimated
furthest instances are drawn first. This is the outer layer of the painter's
algorithm.

The estimate uses two transformed bounding-box corners, not every vertex or
triangle. All primitives from one instance are completed before drawing the
next instance.

## Wireframe Path

For a `WIREFRAME` instance, each triangle contributes its three edges:

1. Each edge is clipped with `clip3d()` against the normalized perspective
   volume, including near and far limits.
2. Surviving endpoints are perspective-divided and mapped around the screen
   centre.
3. The edge is stored as a degenerate triangle whose second and third points
   are equal.
4. The ordinary triangle drawing path reduces that degenerate shape to a line.

Wireframe triangles are not back-face culled, so edges on both sides of a mesh
may be drawn. Shared edges are also submitted more than once.

## Solid Path

For a `SOLID` instance, every source triangle follows these steps:

1. Read its three viewer-space vertices.
2. Calculate a face normal and reject the triangle if it faces away from the
   viewer.
3. Clip the triangle's edges against the front plane with `clip3dpara()`.
4. Perspective-divide surviving points and convert them to screen coordinates.
5. Convert the resulting polygon to a triangle fan.
6. Attach the source triangle's flat colour to every generated triangle.

The current solid clipping helper processes the three triangle edges and
collects their surviving endpoints. It does not share the full-frustum clipping
path used for wireframe edges.

Triangle winding therefore matters for solid meshes. Reversing the three vertex
indices reverses the face normal and changes whether the face passes back-face
culling.

## Projection

Both rendering paths use the same perspective divide after clipping:

```text
screen_x = ((-x / z) * half_width) + half_width
screen_y = ((-y / z) * half_height) + half_height
```

The screen origin is at the SDL renderer's upper-left corner. Adding half the
screen dimensions places the normalized origin at the centre of the window.

## Triangle Sorting

Projected primitives are stored in a temporary list for the current instance.
Each entry contains three screen positions, a colour, and an average Z value.

The list is sorted by increasing Z before drawing. Visible viewer-space Z is
negative, so this draws more distant primitives before nearer ones. The list is
discarded after the instance has been drawn.

## Rasterisation And Presentation

`drawtri()` sorts the three screen vertices by Y and divides a general triangle
into a flat-top and/or flat-bottom section. `filltrapezoid()` then walks from the
top scanline to the bottom scanline and submits one horizontal SDL line per row.

This is software triangle filling built from SDL line primitives. SDL receives
already projected screen coordinates and does not perform 3D transforms,
lighting, clipping, or depth testing.

After `World::render()` returns, `main.cpp` draws the software crosshair and
calls `SDL_RenderPresent()`. It then updates elapsed frame time and places the
calculated frames-per-second value in the window title.

## Visibility Limitations

There is no depth buffer. Correct visibility depends on two levels of painter's
sorting:

- Instances are ordered using an approximate bounding-box distance.
- Triangles are ordered by average Z, but only within their own instance.

This can produce incorrect results for intersecting triangles, cyclic overlap,
large objects whose depth ranges overlap, or triangles from different instances
that need to interleave. Average depth is not sufficient to resolve all of
those cases.

Other current constraints include:

- Solid and wireframe geometry use different clipping paths.
- Solid geometry is clipped at the front plane but not through the same complete
  perspective-volume routine as wireframe edges.
- Lighting is flat per source triangle and does not account for newly generated
  clipping vertices.
- Projection and viewing-volume constants are embedded in the implementation.
- Rendering writes directly through global SDL renderer state, which prevents
  headless inspection of intermediate frames.

These constraints explain the current output and identify the assumptions that
must be considered when changing the renderer; they are not part of the world
file format or a compatibility guarantee.
