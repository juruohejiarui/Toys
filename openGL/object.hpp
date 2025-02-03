#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.hpp"
#include "camera.hpp"

struct Object3DVertix {
    glm::vec3 pos;
    glm::vec3 norv;
    glm::vec2 texc;
    Object3DVertix() = default;
    Object3DVertix(const glm::vec3 &pos, const glm::vec3 &norv, const glm::vec2 &texc);
};

struct Object3D : BaseObject {
protected:
    GLuint vao, vbo, texture;
    Shader *shader;
    
    size_t numVertices;
    Object3DVertix *vertices;

public:
    int getVAO() const;
    int registerObject();
    int unregisterObject();
    int load(const std::string &path);

    Object3D();

    void setShader(Shader *shader);
    Shader *getShader() const;

    void display();
};