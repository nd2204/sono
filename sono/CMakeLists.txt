cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(SonoEngine VERSION 0.1.0 LANGUAGES CXX)

find_package(OpenGL REQUIRED)
add_subdirectory(vendors/glfw)
add_subdirectory(vendors/glad)
add_subdirectory(vendors/imgui)

if(WIN32)
  add_compile_definitions(WIN32_BUILD)
elseif(APPLE)
  add_compile_definitions(OSX_BUILD)
elseif(UNIX)
  add_compile_definitions(UNIX_BUILD)
endif()

file(GLOB SRC_FILES "src/*.cpp")
file(GLOB SNGL_SRC_FILES "src/sngl/*.cpp")

add_executable(${PROJECT_NAME}
  ${SNGL_SRC_FILES}
  ${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  imgui
  glfw
  glad
  OpenGL::GL
)

target_include_directories(${PROJECT_NAME} PRIVATE vendors/glad/include)
target_include_directories(${PROJECT_NAME} PUBLIC includes)