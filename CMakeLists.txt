cmake_minimum_required(VERSION 3.5)

project(LearnOpenGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME GLAD
    GITHUB_REPOSITORY "Dav1dde/glad"
    VERSION 0.1.36
    )


add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)


add_subdirectory(3rdpart/glfw)
