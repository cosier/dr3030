cmake_minimum_required(VERSION 3.6)
project(dr3030)

# This project uses C11 Standard
set( CMAKE_C_STANDARD 11 )

# Treat all warnings as intense errors.
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")

# Main executable has a single file
SET ( SRC_MAIN main.c )

# Setup 2 targets for build output.
# 1. A library for ... library reuse.
# 2. A Executable that utilises the library.
add_executable(dr3030 ${SRC_MAIN})

include_directories(
  ${DR_ROOT}/src
  ${DR_ROOT}/build
  ${DR_ROOT}/vendor/dimappio/src)

# Break into conditionals for audio driver support

# Link Library support: libdimappio
target_link_libraries( dr3030 3030 )

install(TARGETS dr3030
  RUNTIME DESTINATION bin)

