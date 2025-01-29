#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "shader.hpp"
#include "camera.hpp"

struct Object : BaseObject {
private:
    GLuint vao, vbo[3], texture;
    Shader *shader;
    
    size_t numVertices;
    glm::vec3 *vertices, *normalVecs;
    glm::vec2 *texCoords;

public:
    Object();
    friend Object *loadObject(const std::string &path);

    void setShader(Shader *shader);
    Shader *getShader() const;

    void display();
};