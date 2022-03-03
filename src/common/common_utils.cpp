//
// Created by Abao Zhang on 2022/3/3.
//

#include "common_utils.h"
#include <assert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


unsigned int buildShaderProgramFromMemory(const char *vertexShaderSource, const char *fragmentShaderSource) {
    int status;
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), 0, infoLog);
        std::cout << "vertex shader compile error:" << infoLog << std::endl;
        assert(false);
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), 0, infoLog);
        std::cout << "fragment shader compile error:" << infoLog << std::endl;
        assert(false);
    }
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        char infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), 0, infoLog);
        std::cout << "program link error:" << infoLog << std::endl;
        assert(false);
    }
    return program;
}
