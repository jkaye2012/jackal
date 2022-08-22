#!/bin/bash

if [ "$1" == "--help" ]; then
    echo "run_build.sh - Generate build commands and run builds for Jackal"
    echo
    echo "Supported options:"
    echo " --cmake: force cmake to run before building"
    echo " --no-ninja: don't run the ninja build; useful for running cmake only"
    echo ""
    echo "All other arguments are forwarded to the underlying ninja build."
    echo "CMake functionality can be customized by setting environment variables."
    exit 0
fi

SHOULD_CMAKE=0

cd "$(dirname "$0")" || exit 255

if [ ! -d build ]; then
    mkdir build
    SHOULD_CMAKE=1
fi

if [ "$1" == "--cmake" ]; then
    SHOULD_CMAKE=1
    shift
fi

if [ $SHOULD_CMAKE == 1 ]; then
    cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=on -GNinja .
fi

if [ "$1" == "--no-ninja" ]; then
    exit 0
fi

cd build || exit 254
# shellcheck disable=SC2068
ninja $@
