cmake_minimum_required(VERSION 3.15)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(SonoEditor LANGUAGES CXX)

file(GLOB_RECURSE EDITOR_SRC "src/*.cpp")

add_executable(${PROJECT_NAME}
  ${EDITOR_SRC}
)

target_link_libraries(${PROJECT_NAME} PRIVATE sono)
