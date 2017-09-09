cmake_minimum_required(VERSION 3.6)

set (APP_LIB_NAME 3030)
set (APP_EXE_NAME dr30)

set (APP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

project(${APP_LIB_NAME})

set (DR_VERSION_MAJOR 0)
set (DR_VERSION_MINOR 2)
set (DR_VERSION_PATCH 0)
set (DR_VERSION_BUILD $ENV{BUILD_REVISION})
set (DR_VERSION_DATE  $ENV{BUILD_TIMESTAMP})

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

# This project uses C11 Standard
set( CMAKE_C_STANDARD 11 )

# Treat all warnings as intense errors.
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")

set( VENDOR "${APP_ROOT}/vendor")
set( VENDOR_INCLUDES "")

enable_testing ()

add_subdirectory("src/3030")
add_subdirectory("src/main")
add_subdirectory("tests")

configure_file (
  "${APP_ROOT}/src/version.h.in"
  "${APP_ROOT}/src/version.h"
)

include_directories(
  "${CMAKE_SOURCE_DIR}/src"
  "${PROJECT_BINARY_DIR}"
)
