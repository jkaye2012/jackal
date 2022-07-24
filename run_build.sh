#!/bin/bash
# shellcheck disable=SC2039
set -eo pipefail

cd "$(dirname "$0")" || exit 1

cmake -B build -DCMAKE_BUILD_TYPE=Debug .
cd build
make
