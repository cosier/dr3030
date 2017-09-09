cmake_minimum_required(VERSION 3.6)
project(${APP_LIB_NAME})

# This project uses C11 Standard
set( CMAKE_C_STANDARD 11 )

# Treat all warnings as intense errors.
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")

set( _app_defs _DEBUG_ )

# Glob'n all C source + headers under src/
#
# Below we iterate and remove any tmp files that
# Would otherwise break the Make build system.
file(GLOB_RECURSE SRC_LIB *.c *.h)

# Remove tmp files from source tree
foreach ( PATH ${SRC_LIB} )
  string (FIND ${PATH} "#" FOUND)
  if (NOT ${FOUND} EQUAL -1)
    list ( REMOVE_ITEM SRC_LIB ${PATH} )
  endif()
endforeach( PATH )

# Setup 2 targets for build output.
# 1. A library for ... library reuse.
# 2. A Executable that utilises the library.
add_library(${APP_LIB_NAME} STATIC ${SRC_LIB})

# Everybody needs curses
find_library( ncurses_lib ncurses )

# Link Library support: libdimappio
target_include_directories( ${APP_LIB_NAME} PUBLIC ./ )
target_include_directories( ${APP_LIB_NAME} PUBLIC ${VENDOR_INCLUDES} )

target_link_libraries( ${APP_LIB_NAME}
  m
  dimappio
  ubelt
  ${ncurses_lib} )



