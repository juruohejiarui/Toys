#pragma once
#include <GL/glew.h>
#include <string>

struct Shader {
    GLuint program;
    Shader(GLuint program = 0);
    void use();
    void setBool(const std::string &name,  bool value) const;
    void setFloat(const std::string &name, GLfloat value) const;
    void setInt(const std::string &name, GLint value) const;
};

int loadShaders(GLuint &shaderProgram);
int delShaders(GLuint &shaderProgram);