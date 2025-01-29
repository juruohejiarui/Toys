#include "shader.hpp"
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <format>

int readShaderSrc(const std::string &path, std::string &src) {
    std::ifstream ifs;
    ifs.open(path, std::ios::in);
    std::stringstream buf;
    if (!ifs.is_open()) { std::cout << "failed to open file " << path << std::endl; return 0; }
    buf << ifs.rdbuf();
    src = buf.str();
    src.push_back('\0');
    ifs.close();
    return 1;
}

int getShader(uint32_t shaderType, const std::string &path, GLuint &shader) {
    std::string srcStr;
    int res = readShaderSrc(path, srcStr);
    if (!res) return 0;
    auto _shader = glCreateShader(shaderType);
    const char *src = srcStr.c_str();
    glShaderSource(_shader, 1, &src, NULL);
    glCompileShader(_shader);
    int succ;
    char log[512];
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetShaderInfoLog(_shader, 512, NULL, log);
        glDeleteShader(_shader);
        std::cout << std::format("failed to compile shader file {0}, code={1}, log:\n{2}\n", path, succ, log);
        return 0;
    }
    shader = _shader;
    std::cout << std::format("compile shader {0} successfully.\n", path);
    return 1;
}

int loadShaders(GLuint &shaderProgram) {
    GLuint vertShader, fragShader;
    int res = getShader(GL_VERTEX_SHADER, "./vertex.vert", vertShader);
    if (!res) return 0;
    res = getShader(GL_FRAGMENT_SHADER, "./fragment.frag", fragShader);
    if (!res) return 0;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    int succ;
    char log[512];
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &succ);
    if (!succ) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        glDeleteProgram(shaderProgram);
        std::cout << std::format("failed to link shader program, code={0}, log:\n{1}\n", succ, log);
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    std::cout << std::format("successful to link shader program.\n");
    return 1;
}

Shader::Shader(GLuint program) {
    this->program = program;
}

void Shader::use() {
    glUseProgram(this->program);
}
