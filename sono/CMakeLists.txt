cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(SonoEngine LANGUAGES CXX)

set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 1)
set(CMAKE_PROJECT_VERSION_PATCH 0)
set(CMAKE_PROJECT_VERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH})

find_package(OpenGL REQUIRED)
add_subdirectory(vendors/glfw)
add_subdirectory(vendors/glad)
add_subdirectory(vendors/imgui)

# Add debug-specific definition
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(NDEBUG)
endif()

if(WIN32)
  add_compile_definitions(SONO_PLATFORM_WINDOWS)
elseif(APPLE)
  add_compile_definitions(SONO_PLATFORM_MACOS)
elseif(UNIX)
  add_compile_definitions(SONO_PLATFORM_UNIX)
endif()

file(GLOB_RECURSE SOURCE_FILES "core/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC
  imgui
  glfw
  glad
  OpenGL::GL
)

target_include_directories(${PROJECT_NAME} PUBLIC .)