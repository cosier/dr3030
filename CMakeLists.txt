cmake_minimum_required(VERSION 3.6)
project(dimappio)

set (DR_VERSION_MAJOR 0)
set (DR_VERSION_MINOR 2)
set (DR_VERSION_PATCH 0)

if ("$ENV{BUILD_REVISION}" STREQUAL "")
  message (FATAL_ERROR "BUILD_REVISION not specified in ENV")
endif()

if ("$ENV{BUILD_TIMESTAMP}" STREQUAL "")
  message (FATAL_ERROR "BUILD_TIMESTAMP not specified in ENV")
endif()


if ("$ENV{BUILD_STATIC}" STREQUAL "true")
  set (BUILD_SHARED_LIBS OFF)
  set (CMAKE_EXE_LINKER_FLAGS "-static")
endif()

set (DR_VERSION_BUILD $ENV{BUILD_REVISION})
set (DR_VERSION_DATE $ENV{BUILD_TIMESTAMP})

# This project uses C11 Standard
set( CMAKE_C_STANDARD 11 )

set( DR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Treat all warnings as intense errors.
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")

add_subdirectory("vendor/dimappio")
add_subdirectory("src/3030")
add_subdirectory("src/main")
add_subdirectory("tests")

include_directories("vendor/dimappio/src")


enable_testing ()

add_test(NAME test-dr COMMAND test-dr)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
)

include_directories(
  "${CMAKE_SOURCE_DIR}/src"
  "${PROJECT_BINARY_DIR}"
)
