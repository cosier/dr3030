cmake_minimum_required(VERSION 3.6)
project($APP_EXE_NAME)

# This project uses C11 Standard
set( CMAKE_C_STANDARD 11 )

# Treat all warnings as intense errors.
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")

# Main executable has a single file
SET ( SRC_MAIN main.c )

# Setup 2 targets for build output.
# 1. A library for ... library reuse.
# 2. A Executable that utilises the library.
add_executable(${APP_EXE_NAME} ${SRC_MAIN})

include_directories(
  ${APP_ROOT}/src
  ${VENDOR_INCLUDES})

# Break into conditionals for audio driver support

# Link Library support: libdimappio
target_link_libraries(${APP_EXE_NAME}
  ${APP_LIB_NAME})

install(TARGETS ${APP_EXE_NAME}
  RUNTIME DESTINATION bin)

