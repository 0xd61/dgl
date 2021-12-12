#!/usr/bin/env bash

# Stop at errors
set -e

CommonCompilerFlags="-O0 -g -ggdb -fdiagnostics-color=always -std=gnu99 -fno-rtti -fno-exceptions -ffast-math -msse4.1 -msse2
-Wall -Werror -Wconversion
-Wno-writable-strings -Wno-gnu-anonymous-struct
-Wno-padded -Wno-string-conversion
-Wno-error=sign-conversion -Wno-incompatible-function-pointer-types
-Wno-error=unused-variable -Wno-unused-function
-Wno-error=unused-command-line-argument"

CommonLinkerFlags="-Wl,--gc-sections -lm"

if [ -z "$1" ]; then
    OS_NAME=$(uname -o 2>/dev/null || uname -s)
else
    OS_NAME="$1"
fi

echo "Building for $OS_NAME..."

curDir=$(pwd)
srcDir="$curDir"
buildDir="$curDir/build"

[ -d "$buildDir" ] || mkdir -p "$buildDir"

pushd "$buildDir" > /dev/null

# NOTE(dgl): currently only x64 code. For other architectures we have to adjust the intrinsics.
if [ "$OS_NAME" == "GNU/Linux" ] || \
   [ "$OS_NAME" == "Linux" ] || \
   [ "$OS_NAME" == "linux" ]; then
    mkdir -p linux

    echo "Building tests"
    clang $CommonIncludeFlags $CommonCompilerFlags $CommonLinkerFlags -o linux/dgl_test_x64 $srcDir/dgl_test.c

    echo "Testing:"
    ./linux/dgl_test_x64
fi

popd > /dev/null
