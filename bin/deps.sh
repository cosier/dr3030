#!/bin/bash
set -euf -o pipefail

BIN="$( cd  "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $BIN/vars.sh

# Check submodules are initialised
if [[ ! -d $ROOT/vendor/ubelt ]]; then
  cd $ROOT
  git submodule update --init --recursive
fi

###############################
# Build ubelt dependency
$ROOT/vendor/ubelt/bin/build.sh
