#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

struct Shader {
    GLuint program;
    Shader(GLuint program = 0);
    void use();
    GLint getUniformLoc(const std::string &name);
    void setUniform(GLint loc, float value);
    void setUniform(GLint loc, int value);
    void setUniform(GLint loc, const glm::vec3 &value);
    void setUniform(GLint loc, const glm::vec4 &value);
    void setUniform(GLint loc, const glm::mat3 &value);
    void setUniform(GLint loc, const glm::mat4 &value);
};

int loadShaders(GLuint &shaderProgram, const std::string &vertPath, const std::string &fragPath);
int delShaders(GLuint &shaderProgram);