#!/usr/bin/env bash

project_dir=$(pwd)

if [ ! -d "$project_dir/sono" ]; then
  echo "Sono directory not found in the project root. Please ensure you are in the correct directory."
  exit 1
fi

if [ ! cmake --version > /dev/null 2>&1 ]; then
  echo "CMake is not installed. Please install CMake to proceed."
  echo "Refer to the CMake installation guide: https://cmake.org/install/"
  exit 1
fi

if [ ! ninja --version > /dev/null 2>&1 ]; then
  echo "Ninja is not installed. Please install Ninja to proceed."
  echo "Refer to the Ninja installation guide: https://ninja-build.org/"
  exit 1  
fi

engine_dir="${project_dir}/sono"
build_dir="${project_dir}/build"
build_type=Debug

echo "================================================================================"
echo "Configuring Sono"
echo "================================================================================"
echo "Build directory: $build_dir"
echo "Build type: $build_type"

if cmake -B "$build_dir" -S "$project_dir" -G Ninja; then
    echo "CMake configuration successful."
else
    echo "CMake configuration failed."
    exit 1
fi

echo "================================================================================"
echo "Building Sono"
echo "================================================================================"

if cmake --build "$build_dir"; then
    echo "Build successful."
else
    echo "Build failed."
    exit 1
fi