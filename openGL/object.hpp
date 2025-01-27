#pragma once
#include <GL/glew.h>

struct Vertex {
    GLfloat x, y, z;
    GLfloat textureX, textureY;
};

struct Object {
    GLuint vbo, vao, ebo;
    GLuint usage;
    Vertex *vertices;
    GLuint *indices;
    GLuint verticesNum;
    GLuint indicesNum;
};

// add vertices and 
int registerObject(Object *obj);

int drawObject(Object *obj);