cmake_minimum_required(VERSION 3.15)

project(Sono VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_subdirectory(sono)
add_subdirectory(sono-editor)
add_subdirectory(tests)
