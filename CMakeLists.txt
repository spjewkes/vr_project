cmake_minimum_required(VERSION 3.16)

project(
  vr_project
  VERSION 0.1
  LANGUAGES CXX)

option(VR_STRICT_EFFECTIVE_CPP "Enable -Weffc++ warnings" OFF)

function(vr_project_target_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
  endif()

  if(VR_STRICT_EFFECTIVE_CPP AND NOT MSVC)
    target_compile_options(${target} PRIVATE -Weffc++)
  endif()
endfunction()

find_package(SDL2 CONFIG QUIET)

set(VR_PROJECT_SOURCES
    src/collide.cpp
    src/color.cpp
    src/error.cpp
    src/graphics.cpp
    src/instance.cpp
    src/light.cpp
    src/mouse.cpp
    src/options.cpp
    src/parse.cpp
    src/program.cpp
    src/screen.cpp
    src/sound.cpp
    src/vector3d.cpp
    src/world.cpp)

add_library(vr_project_core ${VR_PROJECT_SOURCES})

target_compile_features(vr_project_core PUBLIC cxx_std_14)
target_include_directories(vr_project_core
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
vr_project_target_warnings(vr_project_core)

if(TARGET SDL2::SDL2)
  target_link_libraries(vr_project_core PUBLIC SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
  target_link_libraries(vr_project_core PUBLIC SDL2::SDL2-static)
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
  target_link_libraries(vr_project_core PUBLIC PkgConfig::SDL2)
endif()

add_executable(run_vr_project src/main.cpp)
vr_project_target_warnings(run_vr_project)

if(TARGET SDL2::SDL2main)
  target_link_libraries(run_vr_project PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(run_vr_project PRIVATE vr_project_core)

enable_testing()

add_executable(vr_project_tests tests/test_world_loading.cpp)
target_compile_definitions(
  vr_project_tests PRIVATE VR_PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(vr_project_tests PRIVATE vr_project_core)
vr_project_target_warnings(vr_project_tests)

add_test(NAME world_loading COMMAND vr_project_tests)
