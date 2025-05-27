cmake_minimum_required(VERSION 3.15)
project(SonoCore VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
add_subdirectory(vendors/glfw)
add_subdirectory(vendors/glad)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  OpenGL::GL
  glad
  glfw)

target_include_directories(${PROJECT_NAME} PRIVATE vendors/glad/include)