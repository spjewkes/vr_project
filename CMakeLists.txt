cmake_minimum_required(VERSION 3.16)

project(
  vr_project
  VERSION 0.1
  LANGUAGES CXX)

add_executable(
  run_vr_project
  src/collide.cpp
  src/color.cpp
  src/error.cpp
  src/graphics.cpp
  src/instance.cpp
  src/light.cpp
  src/main.cpp
  src/mouse.cpp
  src/options.cpp
  src/parse.cpp
  src/program.cpp
  src/screen.cpp
  src/sound.cpp
  src/vector3d.cpp
  src/world.cpp)

target_compile_features(run_vr_project PRIVATE cxx_std_14)

if(MSVC)
  target_compile_options(run_vr_project PRIVATE /W4)
else()
  target_compile_options(run_vr_project PRIVATE -Wall -Wextra -pedantic)
endif()

option(VR_STRICT_EFFECTIVE_CPP "Enable -Weffc++ warnings" OFF)

if(VR_STRICT_EFFECTIVE_CPP AND NOT MSVC)
  target_compile_options(run_vr_project PRIVATE -Weffc++)
endif()

find_package(SDL2 CONFIG QUIET)

if(TARGET SDL2::SDL2)
  if(TARGET SDL2::SDL2main)
    target_link_libraries(run_vr_project PRIVATE SDL2::SDL2main)
  endif()
  target_link_libraries(run_vr_project PRIVATE SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
  if(TARGET SDL2::SDL2main)
    target_link_libraries(run_vr_project PRIVATE SDL2::SDL2main)
  endif()
  target_link_libraries(run_vr_project PRIVATE SDL2::SDL2-static)
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
  target_link_libraries(run_vr_project PRIVATE PkgConfig::SDL2)
endif()
