cmake_minimum_required(VERSION 3.5)

project(LearnOpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)

cpmaddpackage(NAME GLAD GITHUB_REPOSITORY "Dav1dde/glad" VERSION 0.1.36)

macro(add_demo targetName sources)

  message(STATUS ${sources})
  add_executable(${targetName} ${sources})

  target_link_libraries(${targetName} PRIVATE glad glfw)

endmacro(add_demo)

add_demo(CreateWindow src/getting_started/creating_window.cpp)
add_demo(HelloTriangle src/getting_started/hello_triangle.cpp)


add_subdirectory(3rdpart/glfw)
