#include "object.hpp"

int registerObject(Object *obj) {
    glGenVertexArrays(1, &obj->vao);
    glGenBuffers(1, &obj->vbo);
    glGenBuffers(1, &obj->ebo);   

    glBindVertexArray(obj->vao);

    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
    glBufferData(GL_ARRAY_BUFFER, obj->verticesNum * sizeof(Vertex), obj->vertices, obj->usage);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->indicesNum * sizeof(GLuint), obj->indices, obj->usage);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // glBindVertexArray(obj->vao[1]);
    // glBindBuffer(GL_ARRAY_BUFFER, obj->vbo[1]);
    // glBufferData(GL_ARRAY_BUFFER, obj->textureNum, obj->texture, obj->usage);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ebo[1]);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->textureIndicesNum, obj->textureIndices, obj->usage);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *)0);

    return 1;
}

int drawObject(Object *obj) {
    glBindVertexArray(obj->vao);
    glDrawElements(GL_TRIANGLES, obj->indicesNum, GL_UNSIGNED_INT, (void *)0);
    glBindVertexArray(0);
    return 1;
}
