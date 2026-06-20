# Architecture

This document describes the current structure of the project. It is intended
as a guide to where behaviour lives, how the main objects relate, and which
ownership assumptions must be preserved when changing the code.

For details of the scene description language, see the
[World File Format](world-format.md). For the graphics stages inside
`World::render()`, see the [Rendering Pipeline](rendering-pipeline.md).

## Build Targets

CMake defines three relevant targets:

| Target | Purpose |
| --- | --- |
| `vr_project_core` | Project library, including parsing, scene data, rendering support, input helpers, collision, and SDL wrappers. |
| `run_vr_project` | Application executable. It supplies `src/main.cpp` and links to `vr_project_core`. |
| `vr_project_tests` | CTest executable covering command-line parsing and loading the bundled worlds. |

SDL2 is a public dependency of `vr_project_core`, so consumers of the core
target inherit the SDL2 link dependency and include requirements.

## Runtime Overview

The program has one main thread and a conventional event/render loop:

```text
Options
  -> construct World
     -> Parser fills scene data
  -> initialise SDL audio
  -> create SDL window and renderer
  -> render initial frame
  -> event and frame loop
     -> process input
     -> test movement collisions
     -> render world
     -> draw pointer
     -> present SDL renderer
  -> close graphics and audio
```

World parsing happens before SDL graphics or audio is initialised. A parse
failure therefore exits without opening a window. At present, audio must also
initialise successfully before the graphics screen is opened.

## Core Data Model

### `World`

`World` is the top-level scene owner. It contains:

- A vector of reusable `Master` meshes.
- A vector of placed `Instance` objects.
- The `Viewer`, including camera transform and environment colours.
- A single positional `Light`.
- A readiness flag set after successful parsing.

Constructing a `World` immediately constructs a `Parser` and parses the named
file into that same object. The parser writes through the mutable `masters()`,
`instances()`, and `user()` accessors.

`World::render()` currently coordinates most of the rendering pipeline. It
transforms instances into viewer space, calculates lighting, sorts geometry,
clips and projects primitives, and calls the low-level drawing API.

### `Master`

A `Master` owns reusable mesh data in local coordinates:

- `vert` contains local-space vertices.
- `poly0`, `poly1`, and `poly2` are parallel arrays containing the three vertex
  indices for every triangle.
- `scale` and `angle` provide a transform shared by all instances of the mesh.
- `id` is the one-based identifier read from the world file.

The three polygon arrays must always have the same size. An element at a given
position in each array describes one triangle.

### `Instance`

An `Instance` places one `Master` in the world. It contains:

- A non-owning `masterptr` pointing to its source mesh.
- Instance position, rotation, scale, colour, specularity, and render style.
- Collision and legacy interaction state derived from `outcome`.
- `world_vert`, containing transformed world-space vertices.
- `view_vert`, containing vertices transformed relative to the current viewer.
- Per-triangle colours and an axis-aligned world-space bounding box.

`Parser` resizes the world's master vector before assigning instance pointers.
This is important: adding or removing masters later may reallocate that vector
and invalidate every `Instance::masterptr`. Code must not mutate the master
vector's size while instances retain these pointers.

`local_to_world()` and `setup_bounds()` run when an instance finishes parsing.
The resulting world vertices and collision bounds remain valid while master and
instance transforms remain unchanged. Changing those transforms at runtime
also requires rebuilding the cached data.

### `Viewer` And `Light`

`Viewer` stores the camera position and rotation, collision radius, sky and
ground colours, and global ambient and directional-light settings.

`World` also owns one `Light`, currently initialised at `(0, 50, 0)` with a
mid-grey colour. This light is not configurable in the world file.

### Maths And Colour

`Vector3d` provides the vector arithmetic, rotations, dot products, cross
products, reflection, and normalization used throughout transforms and
lighting.

`Color` stores floating-point RGB components. Its display accessors convert to
8-bit channels and clamp values to the displayable range. Colour arithmetic is
used to combine material, ambient, diffuse, and specular contributions.

## Modules

| Module | Responsibility |
| --- | --- |
| `main.cpp` | Application lifecycle, SDL event processing, movement, frame timing, and render-loop coordination. |
| `options.*` | Parse help, debug, dump, and world-file command-line options. |
| `parse.*` | Parse the line-oriented world format directly into a `World`. |
| `world.*` | Own scene data and coordinate the software 3D rendering pipeline. |
| `master.hpp` | Define reusable local-space triangle mesh data. |
| `instance.*` | Transform mesh instances, build bounds, and calculate per-triangle lighting. |
| `viewer.hpp` | Hold camera, environment, and global-light state. |
| `vector3d.*` | Provide 3D vector and rotation operations. |
| `color.*` | Provide RGB storage, arithmetic, conversion, and clamping. |
| `collide.*` | Test viewer movement and mouse selection against instance bounds. |
| `graphics.*` | Own SDL window/renderer state and provide 2D drawing primitives. |
| `screen.*` | Map the legacy numeric screen mode to dimensions and create graphics. |
| `mouse.*` | Constrain and draw the software crosshair pointer. |
| `sound.*` | Load and play the collision sound through SDL audio. |
| `program.*` | Interpret legacy interaction text; currently prints arguments instead of launching a process. |
| `error.*` | Centralise error, warning, and optional debug output. |
| `defs.hpp` | Hold shared legacy constants and enums. |

## Input, Collision, And Interaction

`main.cpp` records key-down and key-up events in a small state array. Every
frame it converts the active movement keys into candidate X/Z coordinates.
`check_col()` accepts or rejects that movement by expanding the candidate
viewer position by `radius` on each axis and testing it against every solid
instance's axis-aligned bounding box.

Collision bounds are calculated from transformed world vertices during parsing.
They are broad-phase boxes rather than triangle-accurate geometry. A collision
plays `res/blip.wav` synchronously before returning control to the frame loop.

Mouse selection constructs a world-space ray from the pointer and samples
points along it. Each sample is tested against the same solid instance bounds.
A second click on the same instance calls `program()`. Legacy external process
execution has been removed; non-`solid` outcome text is tokenised and printed.

## SDL And Resource Ownership

Graphics state is held in file-static variables in `graphics.cpp`:

- Window dimensions.
- `SDL_Window*`.
- `SDL_Renderer*`.

Audio sample and playback state is similarly held in file-static variables in
`sound.cpp`. These modules expose procedural APIs rather than lifetime-managed
objects, so there can only be one active graphics context and one active sound
effect.

The collision sound is loaded from `./res/blip.wav`. This path is relative to
the process working directory, so the program currently expects to be launched
from the repository root or another directory with the same relative resource
layout.

## Error Handling

Most parsing operations return the legacy `Status` enum (`Okay` or `Error`).
The `error()` helper prints a message and line number, and callers generally
propagate `Error` back to `main()`.

Not all lower-level failures follow this model. Some SDL drawing operations are
checked only with `assert()`, and an unexpected end of file inside parser
lookahead currently calls `exit()`. Code using the core library should not yet
assume that every failure is recoverable through a returned status.

## Tests

`tests/test_world_loading.cpp` currently checks:

- `--help`, short help, positional world files, and `--file` precedence.
- Successful parsing of all bundled world files.
- Expected master and instance counts.
- Basic mesh-array consistency and instance-to-master relationships.
- Creation of transformed instance vertices.

The tests link the same `vr_project_core` target as the application. They do not
currently create a graphics context or exercise rendering, collision, audio, or
the interactive event loop.

## Current Boundaries

Several boundaries are important when planning changes:

- Scene ownership and rendering orchestration are both concentrated in
  `World`.
- Parsing mutates `World` containers directly rather than producing an
  intermediate scene description.
- Instances depend on stable addresses inside the master vector.
- Graphics and audio state are global to their modules.
- Collision and selection operate on bounding boxes, independently of rendered
  triangle visibility.
- `World::render()` depends directly on the SDL-backed graphics functions, so
  the rendering pipeline cannot currently run without a graphics context.

These are descriptions of the present contracts, not requirements that a future
design preserve them.
