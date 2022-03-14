
CPMAddPackage(
        NAME GLFW
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.3.6
        OPTIONS
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_EXAMPLES OFF"
        "GLFW_BULID_DOCS OFF"
)

cpmaddpackage(NAME GLAD
        GITHUB_REPOSITORY "Dav1dde/glad"
        VERSION 0.1.36
        )

cpmaddpackage(NAME GLM
        GITHUB_REPOSITORY "g-truc/glm"
        GIT_TAG 0.9.9.8
        )

cpmaddpackage(
        NAME stb
        GITHUB_REPOSITORY "nothings/stb"
        GIT_TAG master
)
if (stb_ADDED)
    add_library(stb INTERFACE)

    target_include_directories(stb
            INTERFACE
            ${stb_SOURCE_DIR}
            )
    target_compile_definitions(stb
            INTERFACE
            -DSTB_IMAGE_IMPLEMENTATION
            )
endif ()
