//
// Created by Abao Zhang on 2022/3/3.
//

#include "shader.h"
#include "common_utils.h"
#include <assert.h>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

Shader::Shader() {
    m_programId = 0;
}

Shader Shader::fromMemory(const char *vertexShader, const char *fragmentShader) {
    Shader shader;
    shader.m_programId = buildShaderProgramFromMemory(vertexShader, fragmentShader);
    return shader;
}

Shader Shader::fromPath(const char *vertexPath, const char *fragmentPath) {

    std::string vertexShaderSource, fragmentShaderSource;
    {
        std::ifstream fs;
        std::stringstream ss;
        fs.open(vertexPath, std::ios::in);
        if (!fs.is_open()) {
            std::cout << "can't open vertex shader file" << vertexPath << std::endl;
            assert(false);
        }
        ss << fs.rdbuf();
        vertexShaderSource = ss.str();
    }

    {
        std::ifstream fs;
        std::stringstream ss;
        fs.open(fragmentPath, std::ios::in);
        if (!fs.is_open()) {
            std::cout << "can't open fragment shader file" << fragmentPath << std::endl;
            assert(false);
        }
        ss << fs.rdbuf();
        fragmentShaderSource = ss.str();
    }
    Shader shader;
    shader.m_programId = buildShaderProgramFromMemory(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
    return shader;
}

void Shader::use() const {
    glUseProgram(m_programId);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) {
    glUniform2f(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) {
    glUniform3f(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) {
    glUniform4f(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &value) {
    glUniformMatrix2fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &value) {
    glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
