//
// Created by Abao Zhang on 2022/3/3.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H
#include <glm/glm.hpp>
#include <string>

class Shader {

private:
    Shader();

public:
    static Shader fromMemory(const char *vertexShader, const char *fragmentShader);
    static Shader fromPath(const char *vertexPath, const char *fragmentPath);

public:
    void use() const;

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setVec2(const std::string &name, const glm::vec2 &value);

    void setVec3(const std::string &name, const glm::vec3 &value);

    void setVec4(const std::string &name, const glm::vec4 &value);

    void setMat2(const std::string& name, const glm::mat2& value);

    void setMat3(const std::string& name, const glm::mat3& value);

    void setMat4(const std::string& name, const glm::mat4& value);

private:
    unsigned int m_programId;
};

#endif // LEARNOPENGL_SHADER_H
