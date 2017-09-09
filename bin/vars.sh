#!/bin/bash
set -euf -o pipefail

BIN="$( cd  "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ROOT=$( cd $BIN/../ && pwd )
BUILD_DIR=$ROOT/build

export APP_LIB_NAME=3030
export APP_MAIN_EXE=dr3030

EXE_NAME=$APP_MAIN_EXE
EXE_BUILD=$BUILD_DIR/src/main/$EXE_NAME

