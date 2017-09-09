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

# Break into conditionals for audio driver support
if (APPLE)
  find_library( coreaudio_lib CoreAudio )
  find_library( coremidi_lib CoreMIDI )
  find_library( corefoundation_lib CoreFoundation )
  find_library( audiounit_lib AudioUnit )
  find_library( audiotoolbox_lib AudioToolbox )

  set( MIDI_LIBS
    ${coreaudio_lib}
    ${coremidi_lib}
    ${corefoundation_lib}
    ${audiounit_lib}
    ${audiotoolbox_lib})

  set( MIDI_DEFS __MACOSX_AU__ __MACOSX_CORE__ )

elseif (MSVC)
  set( MIDI_LIBS winmm )
  set( MIDI_DEFS __WINDOWS_DM__ )

elseif (UNIX)
  find_package( ALSA REQUIRED )

  set( MIDI_LIBS ${ALSA_LIBRARY} pthread )
  set( MIDI_DEFS __LINUX_ALSA__ )

endif ()

add_library(ubelt STATIC IMPORTED)
set_property(TARGET ubelt PROPERTY IMPORTED_LOCATION
  ${VENDOR}/ubelt/build/libubelt.a)

add_library(dimappio STATIC IMPORTED)
set_property(TARGET dimappio PROPERTY IMPORTED_LOCATION
  ${VENDOR}/dimappio/build/src/dimappio/libdimappio.a)

# Link Library support: libdimappio
target_include_directories( ${APP_LIB_NAME} PUBLIC ${APP_ROOT}/src )
target_include_directories( ${APP_LIB_NAME} PUBLIC ${VENDOR_INCLUDES} )

target_link_libraries( ${APP_LIB_NAME}
  dimappio
  ncurses
  ubelt
  m
  ${MIDI_LIBS})



