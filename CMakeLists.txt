cmake_minimum_required(VERSION 3.5)

project(LearnOpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)
include(cmake/3rdparty.cmake)


add_library(common STATIC
        src/common/common_utils.cpp
        src/common/shader.cpp
        )
add_custom_command(TARGET common POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
        )

target_link_libraries(common PRIVATE glad glfw stb glm)
target_include_directories(common PUBLIC src/common/)

macro(add_demo targetName sources)

    add_executable(${targetName} ${sources})
    target_link_libraries(${targetName} PRIVATE
            glad
            glfw
            stb
            common
            glm
            )
endmacro(add_demo)

add_demo(CreateWindow src/getting_started/creating_window.cpp)
add_demo(HelloTriangle src/getting_started/hello_triangle.cpp)
add_demo(HelloTriangleVAO src/getting_started/hello_triangle_vao.cpp)
add_demo(HelloTriangleEBO src/getting_started/hello_triangle_ebo.cpp)
add_demo(ShadersUniform src/getting_started/shaders_uniform.cpp)
add_demo(Textures src/getting_started/textures.cpp)
add_demo(Transformations src/getting_started/transformations.cpp)
