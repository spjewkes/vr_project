# Project History

## Original Project

The project began around 1993-94 as a small DOS virtual-reality experiment. It
was developed with Turbo C++ (possibly version 3.0), but the source was written
in C rather than the C++ available at the time. The code already attempted to
separate responsibilities into modules and included extensive comments, even
though it was written after only about a year of experience with C.

The original program used DOS-specific APIs and Borland Graphics Interface
(BGI) drawing. Its world files described a viewer, reusable objects, placed
instances, and strings that could launch external programs when objects were
selected.

## Recovery From Paper

The surviving source was a paper copy. It was converted back into text with the
Prizmo OCR application. Source code was difficult OCR input: formatting was
lost, and digits were sometimes confused with letters. Most problems became
apparent while getting the recovered program to compile.

One source file was missing entirely. It contained common definitions and was
small enough to recreate as `defs.h` from its uses elsewhere.

The uncompiled recovery is preserved in the separate
[old_vr_project repository](https://github.com/spjewkes/old_vr_project). The
earliest revisions of this repository are similar, but include some additional
fixes made while restoring the build.

## Restoration Timeline

### 2018: Port And Refactoring

The first modern restoration replaced DOS and BGI dependencies with SDL2. This
included a new event loop, keyboard and mouse handling, software triangle
filling, collision audio, and an FPS display.

The source was converted from C files to C++, and the code was gradually moved
toward classes and standard-library containers. Parsing was consolidated into a
`Parser` class; scene data became `World`, `Master`, `Instance`, and `Viewer`
types; and vector and colour operations gained dedicated classes.

The world representation changed from explicit edge lists to triangle vertex
indices. Rendering gained per-instance and per-triangle painter-style sorting,
flat lighting, and the office and teapot sample worlds.

The DOS behaviour that launched a process from an object's outcome string was
removed. The modern code retains the string for compatibility and prints the
arguments that would have been used.

### 2023: Build Repair

A missing light implementation was restored and the Makefile was updated so the
project built again with current tools.

### 2026: Tooling And Documentation

The build moved to CMake, with separate Debug and Release wrapper scripts and a
small Makefile wrapper. Formatting and repository checks were added through
pre-commit, followed by an initial CTest suite and Linux/macOS GitHub Actions
jobs.

Command-line handling was improved to support help without a world file and to
accept positional world paths. Project documentation was then expanded to cover
the world-file language, architecture, rendering pipeline, and development
workflow.

## Major Technical Changes

Compared with the recovered DOS version, the current project has:

- Replaced BGI graphics, DOS input, and DOS audio calls with SDL2.
- Converted C translation units to C++ and introduced project classes.
- Replaced many manual arrays and allocations with standard containers.
- Consolidated parsing into one implementation and a `Parser` class.
- Introduced `Vector3d` and `Color` for common mathematical operations.
- Removed explicit mesh edge lists and made polygons triangular.
- Added filled software triangles, back-face culling, flat lighting, and
  clipping.
- Added painter-style instance and triangle sorting.
- Disabled external process execution from world interactions.
- Added CMake, automated tests, formatting hooks, and continuous integration.

## Original Development Direction

The first restoration plan recorded several goals. Some are complete, while
others continue to describe limitations of the current project:

- Replace DOS and BGI facilities with SDL2. This is complete.
- Tidy source formatting and create proper headers. This is substantially
  complete and now enforced by tooling.
- Move toward a more object-oriented structure. Core scene and maths classes
  were introduced, although some procedural and global state remains.
- Remove the 256-colour restriction and improve polygon shading. RGB colour and
  flat lighting now replace the old palette model.
- Add a Z-buffer. This remains unimplemented; rendering still uses painter-style
  sorting.
- Improve collision detection. Collision currently uses axis-aligned instance
  bounds rather than triangle geometry.
- Remove process spawning from object interactions. This is complete.
- Replace or modernise the configuration format. The original line-oriented
  world format remains, although it is now documented.
- Simplify confusing polygon descriptions. Explicit edge lists were removed,
  and all polygons are now triangles.

The project has deliberately retained a software renderer. Hardware-accelerated
3D rendering has been considered, but has not been the main restoration goal.
