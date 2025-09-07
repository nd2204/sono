#!/usr/bin/env bash
project_dir=$(pwd)

if [[ ! -d "$project_dir/sono" || ! -f "$project_dir/build.ps1" ]]; then
  echo "Sono directory not found in the project root. Please ensure you are in the correct directory."
  exit 1
fi

if ! cmake --version > /dev/null 2>&1 ; then
  echo "CMake is not installed. Please install CMake to proceed."
  echo "Refer to the CMake installation guide: https://cmake.org/install/"
  exit 1
fi

uname_output=$(uname -s)
is_win=0
is_mac=0
if [[ "$uname_output" == "Linux" ]]; then
  nop
  # We're building on Linux
elif [[ "$uname_output" == "Darwin" ]]; then
  is_mac=1
  # We're building on macOS
elif [[ "$uname_output" == "MINGW64_NT"* || "$uname_output" == "MSYS_NT"* ]]; then
  is_win=1
  cmake_flags=(
    -DCMAKE_C_COMPILER="x86_64-w64-mingw32-gcc.exe"
    -DCMAKE_CXX_COMPILER="x86_64-w64-mingw32-g++.exe"
  )
else
  echo "Unsupported operating system: $uname_output"
  exit 1
fi

if ! ninja --version > /dev/null 2>&1; then
  echo "Ninja is not installed. Please install Ninja to proceed."
  echo "Refer to the Ninja installation guide: https://ninja-build.org/"
fi

engine_dir="${project_dir}/sono"
build_dir="${project_dir}/build"

build_type=RelWithDebInfo

program_args=()
while [[ $# -gt 0 ]]; do
  if [[ ! $record_args ]]; then
    case $1 in
      "--norun") norun=1; shift ;;
      "--debug") build_type=Debug; shift ;;
      "--release") build_type=Release; shift ;;
      "--rebuild") rebuild=1; shift ;;
      "--test") test=1; shift;;
      "--force" | "-f") force=1; shift ;;
      "--args")
        record_args=1
        shift
        ;;
      *)
        echo "Unknown option: $1"
        shift
        ;;
      esac
  else
    # Everything after -- goes into program_args
    program_args+=("$1")
    shift
  fi
done

if [[ -d "$build_dir" && ! $force ]]; then
  echo "Build directory already exists: $build_dir"
  echo "Skipping CMake configuration..."
else
  echo "================================================================================"
  echo "Configuring Sono"
  echo "================================================================================"

  echo "Build directory: $build_dir"
  echo "Build type: $build_type"

  if cmake -B "$build_dir" -S "$project_dir" -G "Ninja" \
    -DCMAKE_BUILD_TYPE=$build_type \
    ; then
    echo "CMake configuration successful."
  else
    echo "CMake configuration failed."
    exit 1
  fi
fi


echo "================================================================================"
echo "Building Sono"
echo "================================================================================"

if [[ $rebuild ]]; then
  cmake --build "$build_dir" -- clean;
fi

if cmake --build "$build_dir"; then
  echo "Build successful."
else
  echo "Build failed."
  exit 1
fi

echo "================================================================================"
echo "Sono build completed successfully."
echo "================================================================================"

if [[ $is_win -eq 1 ]]; then exe=.exe; else exe= ; fi

if [[ $test -eq 1 ]]; then
  exec="$build_dir/tests/SonoTest$exe ${program_args[@]}"
else
  exec="$build_dir/sono-editor/SonoEditor$exe ${program_args[@]}"
fi


if [[ ! $norun ]]; then
  echo "\"--norun\" flag not set, running the application..."
  $exec
else
  echo "You can find the built files in the $build_dir directory."
  echo "To run the built application, use the following command:"
  echo "  $exec"
fi
