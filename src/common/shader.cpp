//
// Created by Abao Zhang on 2022/3/3.
//

#include "shader.h"
#include "common_utils.h"
#include <glad/glad.h>
Shader::Shader() {
    m_programId = 0;
}

Shader Shader::fromMemory(const char *vertexShader, const char *fragmentShader) {
    Shader shader;
    shader.m_programId = buildShaderProgramFromMemory(vertexShader, fragmentShader);
    return shader;
}

void Shader::use() const {
    glUseProgram(m_programId);
}

void Shader::setBool(const char *name, bool value) const {
    glUniform1i(glGetUniformLocation(m_programId, name), (int)value);
}

void Shader::setInt(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(m_programId, name), value);
}
void Shader::setFloat(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(m_programId, name), value);
}
