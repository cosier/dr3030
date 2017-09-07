cmake_minimum_required(VERSION 3.6)
project(3030-tests)

# This project uses C11 Standard
set( CMAKE_C_STANDARD 11 )

# Treat all warnings as intense errors.
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")

file(GLOB_RECURSE HELPER_LIB helpers/*.c helpers/*.h)

# message ( FATAL_ERROR "HELPER_SRC: ${HELPER_LIB}" )
include_directories(
  "${CMAKE_SOURCE_DIR}/tests"
  "${CMAKE_SOURCE_DIR}/src"
  )

add_library(test_helper STATIC ${HELPER_LIB})
# Test Executable that utilises the library.

add_executable(test-dr test_dr.c)

# Link up main executable with our Library: libdimappio
target_link_libraries( test-dr 3030 test_helper)

