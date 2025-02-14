#include "object.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <format>

int Object3D::getVAO() const { return vao; }

int Object3D::registerObject() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Object3DVertex) * numVertices, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Object3DVertex), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Object3DVertex), (void *)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Object3DVertex), (void *)(sizeof(glm::vec3) * 2));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return 0;
}

int Object3D::unregisterObject() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return 0;
}

int Object3D::load(const std::string &path) {
    float x, y, z;
    std::string line;
    std::ifstream ifs(path, std::ios::in);

    if (!ifs.is_open()) {
        std::cout << std::format("Failed to load .obj file {0}.\n", path);
        return 0;
    }

    std::vector<glm::vec3> vert, normVecs;
    std::vector<glm::vec2> texC;

    std::vector<Object3DVertex> vertices;
    
    while (!ifs.eof()) {
        std::getline(ifs, line);
        if (line.compare(0, 2, "v ") == 0) {
            std::stringstream ss(line.erase(0, 1));
            ss >> x >> y >> z;
            vert.push_back(glm::vec3(x, y, z));
        } else if (line.compare(0, 2, "vt") == 0) {
            std::stringstream ss(line.erase(0, 2));
            ss >> x >> y;
            texC.push_back(glm::vec2(x, y));
        } else if (line.compare(0, 2, "vn") == 0) {
            std::stringstream ss(line.erase(0, 2));
            ss >> x >> y >> z;
            normVecs.push_back(glm::vec3(x, y, z));
        } else if (line[0] == 'f') {
            std::string cor;
            std::stringstream ss(line.erase(0, 2));
            unsigned int defNorBit = 0;
            for (int i = 0; i < 4; i++) {
                std::getline(ss, cor, ' ');
                size_t vertId, norId;
                Object3DVertex vertex;
                if (cor.length() == 0 || cor == "\r") {
                    if (i == 3) break;
                    printf("invalid face.\n");
                    return 0;
                }
                if (cor.find('/') == std::string::npos) {
                    vertId = std::stoi(cor) - 1;
                } else {
                    std::stringstream corss(cor);
                    std::string v, t, n;
                    std::getline(corss, v, '/');
                    std::getline(corss, t, '/');
                    std::getline(corss, n, '/');

                    vertId = std::stoi(v) - 1;

                    if (t.length()) vertex.texc = texC[std::stoi(t) - 1];
                    if (n.length()) vertex.norv = normVecs[std::stoi(n) - 1], defNorBit |= (1 << i);
                }
                vertex.pos = vert[vertId];
                vertices.push_back(vertex);
                auto calcNorv = [&vertices, &defNorBit]() {
                    Object3DVertex &a = vertices[vertices.size() - 3], &b = vertices[vertices.size() - 2], &c = vertices[vertices.size() - 1];
                    if (~defNorBit & 0x1) a.norv = glm::normalize(glm::cross(b.pos - a.pos, c.pos - a.pos));
                    if (~defNorBit & 0x2) b.norv = glm::normalize(glm::cross(c.pos - b.pos, a.pos - b.pos));
                    if (~defNorBit & 0x4) c.norv = glm::normalize(glm::cross(a.pos - c.pos, b.pos - c.pos));
                };
                if (i == 2) calcNorv();
                if (i == 3) {
                    vertices.push_back(vertices[vertices.size() - 4]);
                    vertices.push_back(vertices[vertices.size() - 3]);
                    defNorBit = (defNorBit & 1) | (defNorBit >> 1);
                    calcNorv();
                }
            }
        }
    }
    numVertices = vertices.size();
    this->vertices = new Object3DVertex[numVertices];
    memcpy(this->vertices, &vertices[0], numVertices * sizeof(Object3DVertex));
    printf("numVertices = %ld\n", numVertices);
    return 1;
}

Object3D::Object3D() : BaseObject(ObjectType::Object3D) {
    vao = texture = 0;
    memset(&vbo, 0, sizeof(vbo));

    shader = nullptr;

    numVertices = 0;
    vertices = nullptr;
}

void Object3D::setShader(Shader *shader) {
    this->shader = shader;
    shaderModelLoc = shader->getUniformLoc("model");
}

Shader *Object3D::getShader() const { return shader; }

void Object3D::display() {
    glBindVertexArray(vao);
    shader->use();
    shader->setUniform(shaderModelLoc, getModelMat());
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindVertexArray(0);
}

size_t Object3D::getNumVertices() const { return numVertices; }

const Object3DVertex &Object3D::getVertex(size_t index) const { return vertices[index]; }

Object3DVertex::Object3DVertex(const glm::vec3 &pos, const glm::vec3 &norv, const glm::vec2 &texc) {
    this->pos = pos;
    this->norv = norv;
    this->texc = texc;
}
