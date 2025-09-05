cmake_minimum_required(VERSION 3.15)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(SonoTest LANGUAGES CXX)

file(GLOB_RECURSE MATH_TEST_SRC "sono/math/*.cpp")
add_executable(${PROJECT_NAME}
  main.cpp
  ${MATH_TEST_SRC}
)
target_link_libraries(${PROJECT_NAME} PRIVATE sono)
target_include_directories(${PROJECT_NAME} PRIVATE vendors/doctest)

# TODO: Move this 2 line to sono-editor
add_executable(SonoEditor sono/src/main.cpp)
target_link_libraries(SonoEditor PRIVATE sono)
