cmake_minimum_required(VERSION 3.5)

project(LearnOpenGL LANGUAGES CXX)


include(cmake/CPM.cmake)
include(cmake/3rdparty.cmake)

file(GLOB_RECURSE res_files RELATIVE ${CMAKE_CURRENT_LIST_DIR} "res/**")

foreach (file ${res_files})
    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${file}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}/${file}"
            DEPENDS "${CMAKE_CURRENT_LIST_DIR}/${file}"
    )
endforeach ()

list(TRANSFORM res_files PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")

message(STATUS ${res_files})

add_custom_target(CopyResource ALL
        DEPENDS ${res_files}
        )

add_library(common STATIC
        src/common/common_utils.cpp
        src/common/shader.cpp
        src/common/player_camera.cpp
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
    add_dependencies(${targetName} CopyResource)
endmacro(add_demo)

add_demo(CreateWindow src/getting_started/creating_window.cpp)
add_demo(HelloTriangle src/getting_started/hello_triangle.cpp)
add_demo(HelloTriangleVAO src/getting_started/hello_triangle_vao.cpp)
add_demo(HelloTriangleEBO src/getting_started/hello_triangle_ebo.cpp)
add_demo(ShadersUniform src/getting_started/shaders_uniform.cpp)
add_demo(Textures src/getting_started/textures.cpp)
add_demo(Transformations src/getting_started/transformations.cpp)
add_demo(CoordinateSystems src/getting_started/coordinate_system.cpp)
add_demo(Camera src/getting_started/camera.cpp)

add_demo(BasicLighting src/lighting/basic_lighting.cpp)
add_demo(BasicGouraudLighting src/lighting/basic_gouraud_lighting.cpp)
