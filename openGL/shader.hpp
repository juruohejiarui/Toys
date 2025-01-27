#pragma once
#include <GL/glew.h>
#include <string>

struct Shader {
    GLuint program;
    Shader(GLuint program);
    void use();
    void setBool(const std::string &name,  bool value) const;
    void setFloat(const std::string &name, GLfloat value) const;
    void setInt(const std::string &name, GLint value) const;
};

int loadShaders(GLuint &shaderProgram);