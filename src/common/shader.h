//
// Created by Abao Zhang on 2022/3/3.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

class Shader {

private:
    Shader();

public:
    static Shader fromMemory(const char *vertexShader, const char *fragmentShader);

public:
    void use() const;

    // utility uniform functions
    void setBool(const char *name, bool value) const;

    void setInt(const char *name, int value) const;

    void setFloat(const char *name, float value) const;


private:
    unsigned int m_programId;
};

#endif // LEARNOPENGL_SHADER_H
