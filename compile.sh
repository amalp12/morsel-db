#!/bin/bash
# This script helps you set up MLIR
# Prerequisites: cmake, c compiler, make
# also, make sure you do `git submodule update --init --recursive`
# in this repo to get llvm-project under thirdparty

# Set up MLIR
LLVM_REPO=../mlir-hello/thirdparty/llvm-project
BUILD_DIR=$LLVM_REPO/build
INSTALL_DIR=$LLVM_REPO/install
CCACHE_PREFIX=distcc

set -e


  # -DLLVM_DIR=$LLVM_REPO/build/lib/cmake/llvm \
  # -DMLIR_DIR=$LLVM_REPO/build/lib/cmake/mlir \
# set up mlir-basic
mkdir -p build
cd build
cmake -G Ninja .. \
  -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER_LAUNCHER=ccache \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  #-DLLVM_ENABLE_LLD=ON

#cmake --build . --target basic-opt externfunc externfuncshared
cmake --build . --target dbapp morsel_test

cd ..
# test run

#  ./build/bin/basic-opt ./test/Basic/print.mlir > print.ll
# $BUILD_DIR/bin/lli print.ll
# Compile manually
# Generate lli using basic-opt <mlir-file>
# use llc -filetype=obj -relocation-model=pic <lli-file> -o <obj-file>
# use clang++ -fPIE -fuse-ld=lld <lli-obj-file> <..optional libs.a file>  -o <exec-file>
