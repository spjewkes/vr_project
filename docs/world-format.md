# World File Format

World files describe the initial viewer, reusable meshes, and placed instances
of those meshes. The bundled files in [`res/`](../res/) use this format and can
serve as larger examples.

The format is a line-oriented, case-sensitive text format. A typical file has
these blocks in order:

1. `.userdefs` defines the viewer and environment colours.
2. `.masterdefs` defines reusable triangle meshes.
3. `.instancedefs` places and renders instances of those meshes.

The user block must appear before the master block. Instances can only refer to
masters that have already been defined, so keeping the blocks in the order
above is strongly recommended.

## General Syntax

- Keywords and symbolic values are case-sensitive.
- Spaces and tabs may be used for indentation.
- Blank lines are ignored between definitions.
- A line whose first non-whitespace character is `#` is a comment.
- Inline comments are not supported.
- Each definition must fit on one line.
- Vectors use three comma-separated numbers: `x, y, z`.
- Angles are measured in degrees.
- Mesh, vertex, polygon, and master references are numbered from 1 in the
  file.

For example:

```text
# This entire line is a comment.
location = 0.0, 10.0, 40.0
```

## Coordinates And Transforms

The current coordinate convention is:

- Positive X points right.
- Positive Y points up.
- With a direction of `0, 0, 0`, the viewer looks along negative Z.
- An angle vector contains rotations around X, Y, and Z, in that order.

For each instance, local mesh vertices are transformed in this order:

1. Master scale.
2. Instance scale.
3. Master rotation.
4. Instance rotation.
5. Instance translation.

There is no declared real-world unit. Values are interpreted consistently as
world-space units; the bundled worlds are the best guide to their practical
scale.

## Viewer Definitions

The `.userdefs` block sets the initial viewer and background colours:

```text
.userdefs
location = 0.0, 0.0, 40.0
direction = 0.0, 0.0, 0.0
radius = 2.5
sky = 0.8, 0.8, 1.0
ground = 1.0, 0.8, 0.6
.end_userdefs
```

| Field | Meaning |
| --- | --- |
| `location` | Initial viewer position in world space. |
| `direction` | Initial X, Y, and Z rotation in degrees. |
| `radius` | Radius used for viewer collision tests. |
| `sky` | Red, green, and blue components of the upper background. |
| `ground` | Red, green, and blue components of the lower background. |

Colour components should be between `0.0` and `1.0`. Values outside that range
are currently clamped when converted for display rather than rejected while
parsing.

The fields may appear in any order. Although the parser supplies defaults for
omitted fields, specifying all five makes a world file self-contained and
predictable.

## Master Definitions

A master is a reusable triangle mesh. The block starts with the exact number
of masters that follow:

```text
.masterdefs
no_objects = 1

master_no = 1
.objectdef
no_vertices = 3
1 = -1.0, -1.0, 0.0
2 =  1.0, -1.0, 0.0
3 =  0.0,  1.0, 0.0
no_polygons = 1
1 = 1, 2, 3
.objectend
scale = 1.0, 1.0, 1.0
angle = 0.0, 0.0, 0.0

.end_masterdefs
```

### Master Numbers

`master_no` identifies a master for later instance references. Its value must
be between 1 and `no_objects`. Define masters sequentially as `1` through
`no_objects`; this matches the parser's storage model and all bundled worlds.

The number of `master_no` definitions must exactly match `no_objects`.

### Object Geometry

Each `.objectdef` contains a vertex list followed by a polygon list:

```text
.objectdef
no_vertices = 4
1 = -1.0,  1.0, 0.0
2 =  1.0,  1.0, 0.0
3 =  1.0, -1.0, 0.0
4 = -1.0, -1.0, 0.0

no_polygons = 2
1 = 1, 2, 3
2 = 1, 3, 4
.objectend
```

`no_vertices` and `no_polygons` should both be positive. Each count must be
followed by exactly that many numbered definitions.

A vertex definition has this form:

```text
vertex-number = x, y, z
```

A polygon definition has this form:

```text
polygon-number = first-vertex, second-vertex, third-vertex
```

Only triangles are supported. Every vertex reference must be between 1 and
`no_vertices`.

Vertex order determines which side of a triangle faces the viewer and how it
is lit. Solid rendering performs back-face culling, so reverse the three
references if a surface is visible from the wrong side. Wireframe rendering
does not perform this culling.

### Master Transforms

The following fields are optional after `.objectend` and before the next
`master_no` or `.end_masterdefs`:

| Field | Default | Meaning |
| --- | --- | --- |
| `scale` | `1.0, 1.0, 1.0` | Scale applied to every instance of the master. |
| `angle` | `0.0, 0.0, 0.0` | Rotation applied to every instance, in degrees. |

## Instance Definitions

An instance places a master in the world and supplies its appearance and
collision behaviour. The block declares the exact number of instances:

```text
.instancedefs
no_instances = 1

master_no = 1
location = 0.0, 0.0, -40.0
scale = 10.0, 10.0, 10.0
angle = 0.0, 0.0, 0.0
color = WHITE
specularity = 50.0 %
style = SOLID
outcome = "solid"

.end_instancedefs
```

Each instance starts with `master_no`. The value must refer to a master from
the preceding `.masterdefs` block. The number of instance definitions must
exactly match `no_instances`.

The remaining fields may appear in any order:

| Field | Required | Default | Meaning |
| --- | --- | --- | --- |
| `location` | No | `0.0, 0.0, 0.0` | Translation in world space. |
| `scale` | No | `1.0, 1.0, 1.0` | Scale applied after the master scale. |
| `angle` | No | `0.0, 0.0, 0.0` | Rotation applied after the master rotation. |
| `color` | Yes | None | Base colour used for rendering and lighting. |
| `specularity` | Yes | None | Specular intensity from `0.0` to `100.0`. |
| `style` | Yes | None | Either `SOLID` or `WIREFRAME`. |
| `outcome` | No | Empty | Collision and interaction behaviour. |

The `%` after `specularity` is optional. Both of these are valid:

```text
specularity = 50.0
specularity = 50.0 %
```

### Colours

`color` accepts one of the following names or its numeric alias:

| Value | Name | Value | Name |
| --- | --- | --- | --- |
| `0` | `BLACK` | `8` | `DARKGREY` |
| `1` | `BLUE` | `9` | `LIGHTBLUE` |
| `2` | `GREEN` | `10` | `LIGHTGREEN` |
| `3` | `CYAN` | `11` | `LIGHTCYAN` |
| `4` | `RED` | `12` | `LIGHTRED` |
| `5` | `MAGENTA` | `13` | `LIGHTMAGENTA` |
| `6` | `BROWN` | `14` | `YELLOW` |
| `7` | `LIGHTGREY` | `15` | `WHITE` |

The symbolic names are clearer and are preferred in new files.

### Outcomes And Collision

`outcome` is a double-quoted string:

| Value | Behaviour |
| --- | --- |
| Omitted or `""` | The instance does not participate in collision tests. |
| `"ignore"` | The instance does not participate in collision tests. |
| `"solid"` | The instance has solid axis-aligned bounds. |
| Any other non-empty string | The instance is solid and retains the string as a legacy interaction command. |

External command execution from legacy outcome strings is no longer
implemented. When such an object is selected, the current program only prints
the parsed command arguments.

Collision uses each transformed instance's axis-aligned bounding box, not its
individual triangles. Rotated objects can therefore have a collision volume
larger than their visible geometry.

## Minimal Complete Example

```text
.userdefs
location = 0.0, 0.0, 20.0
direction = 0.0, 0.0, 0.0
radius = 1.0
sky = 0.2, 0.3, 0.5
ground = 0.2, 0.2, 0.2
.end_userdefs

.masterdefs
no_objects = 1

master_no = 1
.objectdef
no_vertices = 3
1 = -1.0, -1.0, 0.0
2 =  1.0, -1.0, 0.0
3 =  0.0,  1.0, 0.0
no_polygons = 1
1 = 1, 2, 3
.objectend

.end_masterdefs

.instancedefs
no_instances = 1

master_no = 1
location = 0.0, 0.0, -20.0
scale = 5.0, 5.0, 5.0
color = LIGHTGREEN
specularity = 25.0 %
style = SOLID
outcome = "solid"

.end_instancedefs
```

Run a world with:

```sh
./build/debug/run_vr_project --file path/to/world.txt
```

Parsing errors are reported with a line number. Because the parser is
line-oriented, first check spelling, case, block ordering, delimiters, and the
declared counts when diagnosing an error.
