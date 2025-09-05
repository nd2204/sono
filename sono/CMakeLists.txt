cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(sono LANGUAGES CXX)

set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 1)
set(CMAKE_PROJECT_VERSION_PATCH 0)
set(CMAKE_PROJECT_VERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH})

option(SN_NO_MEMTRACKING "Enable memory allocation tracking" ON)
option(SN_BUILD_DLL "Build dynamic lib" OFF)

if(WIN32)
  add_compile_definitions(SONO_PLATFORM_WINDOWS)
elseif(APPLE)
  add_compile_definitions(SONO_PLATFORM_MACOS)
elseif(UNIX)
  add_compile_definitions(SONO_PLATFORM_UNIX)
endif()

find_package(OpenGL REQUIRED)
add_subdirectory(vendors/glfw)
add_subdirectory(vendors/glad)
add_subdirectory(vendors/imgui)
add_subdirectory(vendors/glm)

# Add debug-specific definition
set(TARGET_NAME ${PROJECT_NAME})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(SN_NDEBUG)
endif()

file(GLOB_RECURSE CORE_SRC "core/*.cpp")
file(GLOB_RECURSE RENDER_SRC "render/*.cpp")
file(GLOB_RECURSE RENDER_BACKEND_SRC "render-backend/*.cpp")

add_library(${TARGET_NAME} STATIC
  ${CORE_SRC}
  ${RENDER_SRC}
  ${RENDER_BACKEND_SRC}
)

target_link_libraries(${TARGET_NAME} PUBLIC
  imgui
  glfw
  glad
  glm
  OpenGL::GL
)

target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(${TARGET_NAME} PUBLIC .)
target_include_directories(${TARGET_NAME} PUBLIC vendors/stb)
