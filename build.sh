#!/usr/bin/env bash

project_dir=$(pwd)

if [ ! -d "$project_dir/sono" ]; then
    echo "Sono directory not found in the project root. Please ensure you are in the correct directory."
    exit 1
fi

engine_dir="${project_dir}/sono"
build_dir="${project_dir}/sono/build"

echo "================================================================================"
echo "Configuring Sono"
echo "================================================================================"
echo "Build directory: $build_dir"

if cmake -B "$build_dir" -S "$engine_dir" -G Ninja; then
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