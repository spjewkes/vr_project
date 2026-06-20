# Development

This page covers the local build, test, and code-quality workflow. For module
responsibilities and data ownership, see [Architecture](architecture.md).

## Requirements

Building the application requires:

- CMake 3.16 or newer.
- A compiler with C++14 support.
- SDL2 development files.
- `pkg-config` when SDL2 is not available through a CMake package configuration.

CMake first looks for an SDL2 package that provides an imported target. If one
is not available, it falls back to the `sdl2` module exposed by `pkg-config`.

The repository's development checks additionally use Python 3, pre-commit,
clang-format, and cmake-format. The `cmake-format` command is supplied by the
`cmakelang` Python package.

## Debug Build

The standard debug build uses the wrapper script and writes to `build/debug`:

```sh
./build_debug.sh
```

The equivalent direct CMake commands are:

```sh
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug
```

## Release Build

The release wrapper writes to `build/release`:

```sh
./build_release.sh
```

The equivalent direct commands are:

```sh
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/release
```

Both wrapper scripts accept additional arguments for `cmake --build`. Their
build directory can also be overridden through `BUILD_DIR`:

```sh
BUILD_DIR=build/custom ./build_debug.sh --parallel
```

## Makefile Wrapper

The Makefile is a convenience wrapper around CMake. Its default build directory
is `build`, rather than `build/debug`:

```sh
make
make release
make build
```

Override the directory or initial configuration type when needed:

```sh
make BUILD_DIR=build/local BUILD_TYPE=Debug configure
```

The build scripts are preferred in documentation because they keep Debug and
Release products separate.

## Running The Application

Run from the repository root so the current relative audio resource path,
`./res/blip.wav`, resolves correctly:

```sh
./build/debug/run_vr_project --file res/cube.txt
```

A world file can also be passed positionally:

```sh
./build/debug/run_vr_project res/office.txt
```

The supported command-line options are:

| Option | Purpose |
| --- | --- |
| `-h`, `--help` | Print usage and exit. |
| `-f FILE`, `--file FILE` | Load the specified world file. |
| `FILE` | Load a positional world file when `--file` is not supplied. |
| `-b`, `--debug` | Enable verbose parser and function debug output. |
| `-d`, `--dump` | Print parsed master and instance data before opening the window. |

When both `--file` and a positional file are supplied, `--file` takes
precedence.

## Tests

Build before running CTest. The Debug and Release suites are invoked separately:

```sh
ctest --test-dir build/debug --output-on-failure
ctest --test-dir build/release --output-on-failure
```

The current `world_loading` test covers command-line parsing and loading the
three bundled worlds. Rendering and the interactive SDL loop are not currently
exercised by CTest.

## Compiler Warnings

All project targets enable the normal high-warning set: `/W4` with MSVC, or
`-Wall -Wextra -pedantic` with other supported compilers.

An additional Effective C++ warning mode is available on non-MSVC compilers:

```sh
cmake -S . -B build/effective-cpp \
  -DCMAKE_BUILD_TYPE=Debug \
  -DVR_STRICT_EFFECTIVE_CPP=ON
cmake --build build/effective-cpp
```

This mode is optional because `-Weffc++` is considerably noisier than the
default warning set.

## Pre-Commit Checks

Install the hooks once in a fresh checkout:

```sh
pre-commit install
```

Run every configured check manually with:

```sh
pre-commit run --all-files
```

The hooks:

- Remove trailing whitespace and ensure final newlines.
- Reject unresolved merge-conflict markers.
- Format C and C++ sources with clang-format.
- Format CMake files with cmake-format.
- Check shell-script syntax.

Some hooks modify files. Review their changes and rerun the command until all
checks pass.

## Continuous Integration

`.github/workflows/ci.yml` runs on pushes to `master` or `main` and on pull
requests. Its Linux and macOS jobs:

1. Install SDL2 and the formatting tools.
2. Run pre-commit across the repository.
3. Build and test Debug.
4. Build and test Release.

Keeping the documented local commands aligned with these jobs helps prevent
environment-specific build failures.

## Repository Layout

| Path | Purpose |
| --- | --- |
| `src/` | Application, scene, renderer, SDL wrapper, and utility sources. |
| `tests/` | CTest-backed test executable sources. |
| `res/` | Sample worlds, collision audio, screenshot, and the OBJ conversion script. |
| `docs/` | World-format, architecture, rendering, development, and history documentation. |
| `scripts/precommit/` | Repository-local pre-commit helpers. |
| `.github/workflows/` | Continuous-integration configuration. |

When adding a new C++ source file to the core library, also add it to
`VR_PROJECT_SOURCES` in `CMakeLists.txt`.
