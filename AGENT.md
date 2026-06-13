# Agent Notes

This file records project context and a practical improvement roadmap for future
work on this repository.

## Project Snapshot

- The project is a revived early-1990s software 3D renderer, now C++14 and SDL2.
- Build system is CMake, with a Makefile wrapper kept for convenience.
- Pre-commit hooks are configured and installed locally for this checkout.
- The source tree has already had an all-files pre-commit formatting pass.
- Main sample worlds live in `res/cube.txt`, `res/office.txt`, and
  `res/teapot.txt`.
- The executable target is `run_vr_project`.

## Common Commands

Debug build:

```sh
./build_debug.sh
```

Release build:

```sh
./build_release.sh
```

Equivalent direct CMake debug build:

```sh
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug
```

Run a sample world:

```sh
./build/debug/run_vr_project --file res/cube.txt
```

Headless smoke run with SDL dummy drivers:

```sh
SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy ./build/debug/run_vr_project --file res/cube.txt
```

Run pre-commit:

```sh
pre-commit run --all-files
```

Install hooks in a fresh checkout:

```sh
pre-commit install
```

## Current Build And Tooling State

- `CMakeLists.txt` finds SDL2 through CMake package config first, then
  `pkg-config`.
- `build_debug.sh` writes to `build/debug`.
- `build_release.sh` writes to `build/release`.
- `.clang-format` controls C++ formatting.
- `.pre-commit-config.yaml` runs:
  - whitespace/final-newline fixer
  - merge-conflict marker check
  - `clang-format`
  - `cmake-format`
  - shell syntax checks

## Important Code Notes

- `World::render()` currently owns too much rendering pipeline logic:
  object preparation, visibility decisions, clipping, projection, sorting, and
  drawing.
- The parser is concentrated in `src/parse.cpp` and still uses manual
  line-pointer scanning.
- `sound.cpp` loads `./res/blip.wav`, so audio resource loading depends on the
  current working directory.
- `Instance::masterptr` points into the world's master vector. This is workable
  with current parse flow, but it is fragile if ownership or vector mutation
  changes.
- Graphics and sound systems use global/static SDL state.
- There is no automated test suite yet.

## Improvement Roadmap

### First Priority

1. Add automated tests.
   Start with parser/smoke tests for `res/cube.txt`, `res/office.txt`, and
   `res/teapot.txt`, then add unit tests for `Vector3d`, `Color`, clipping, and
   collision boxes.

2. Add CI.
   Use GitHub Actions to run pre-commit, configure/build Debug and Release, and
   run parser or dummy-SDL smoke tests on Linux and macOS.

3. Fix resource path handling.
   Avoid hard-coded `./res/blip.wav`. Prefer a resource directory resolved from
   command-line options, executable location, or CMake-provided build paths.

4. Improve command-line behavior.
   Make `--help` exit without requiring a world file. Consider accepting a
   positional world file as well as `--file`.

### Correctness And Robustness

5. Replace parser internals with safer token helpers.
   The parser should reject partial numeric tokens such as `1abc`, report exact
   locations, and be easy to test without starting SDL.

6. Remove `exit()` from lower-level code.
   Return errors upward so tests can assert failures and callers can decide how
   to handle them.

7. Replace debug-only `assert()` checks where runtime failures matter.
   SDL errors, file/resource failures, and invalid world data should report
   errors in release builds too.

8. Add explicit world-data validation.
   Check empty vertex lists, invalid polygon arrays, missing `masterptr`, and
   out-of-range references before render-time access.

9. Improve audio lifecycle.
   Open the audio device once, queue or play effects as needed, and close the
   device during shutdown.

### Rendering Work

10. Split the rendering pipeline.
    Separate transformation, clipping, projection, sorting, and rasterization
    from `World::render()`.

11. Add a framebuffer or render-target abstraction.
    This would make rendering easier to test and enable screenshot regression
    tests.

12. Add z-buffering.
    Painter-style sorting will fail for intersecting or overlapping geometry.
    A software z-buffer would be a major correctness improvement.

13. Improve solid triangle rasterization.
    Add tests around degenerate, flat-top, flat-bottom, and off-screen
    triangles.

### Architecture

14. Replace raw master pointers in `Instance`.
    Use master indices or stable ownership to avoid pointer invalidation risks.

15. Untangle header dependencies.
    In particular, avoid circular-style relationships between `defs.hpp` and
    vector-related types.

16. Encapsulate SDL graphics and sound state.
    Replace static globals with small lifetime-managed objects.

17. Replace legacy C-style APIs gradually.
    Prefer standard library types and explicit error handling where it reduces
    risk.

### Documentation And Project Quality

18. Document the world file format.
    Include valid blocks, required fields, coordinate conventions, and examples.

19. Add CMake resource/install support.
    Make it easy to run from build and install layouts without current-directory
    assumptions.

20. Add sanitizer build options.
    AddressSanitizer and UndefinedBehaviorSanitizer would be useful for this
    old pointer/index-heavy code.

## Recommended Next Step

The best next piece of work is a small test harness focused on parsing and smoke
loading sample worlds without entering the SDL event loop. That gives useful
coverage before refactoring parser, resource loading, or rendering internals.
