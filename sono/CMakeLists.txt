cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(SonoCore VERSION 0.1.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED)
add_subdirectory(vendors/glfw)
add_subdirectory(vendors/glad)

if(APPLE)
  add_compile_definitions(MACOS_BUILD)
endif()

add_executable(${PROJECT_NAME}
  src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  OpenGL::GL
  glad
  glfw
)

target_include_directories(${PROJECT_NAME} PRIVATE vendors/glad/include)