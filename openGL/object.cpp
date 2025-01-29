#include "object.hpp"
#include <fstream>
#include <sstream>
#include <cstring>

Object *loadObject(const std::string &path) {
    float x, y, z;
    std::string line;
    std::ifstream ifs(path, std::ios::in);

    Object *obj = new Object();

    std::vector<glm::vec3> vertices, normVecs;
    std::vector<glm::vec2> texCoords;

    std::vector<glm::vec3> triVert, triNormVecs;
    std::vector<glm::vec2> triTexCoords;
    
    while (!ifs.eof()) {
        std::getline(ifs, line);
        if (line.compare(0, 2, "v ") == 0) {
            std::stringstream ss(line.erase(0, 1));
            ss >> x >> y >> z;
            vertices.push_back(glm::vec3(x, y, z));
        } else if (line.compare(0, 2, "vt") == 0) {
            std::stringstream ss(line.erase(0, 2));
            ss >> x >> y;
            texCoords.push_back(glm::vec2(x, y));
        } else if (line.compare(0, 2, "vn") == 0) {
            std::stringstream ss(line.erase(0, 2));
            ss >> x >> y >> z;
            normVecs.push_back(glm::vec3(x, y, z));
        } else if (line[0] == 'f') {
            std::string cor, v, t, n;
            std::stringstream ss(line.erase(0, 2));
            for (int i = 0; i < 3; i++) {
                std::getline(ss, cor, ' ');
                std::stringstream corss(cor);
                std::getline(corss, v, '/');
                std::getline(corss, t, '/');
                std::getline(corss, n, '/');

                size_t vertId = std::stoi(v) - 1, texId = std::stoi(t) - 1, norId = std::stoi(n) - 1;

                triVert.push_back(vertices[vertId]);
                triNormVecs.push_back(normVecs[norId]);
                triTexCoords.push_back(texCoords[texId]);
            }
        }
    }
    obj->numVertices = triVert.size();
    obj->vertices       = new glm::vec3[obj->numVertices];
    obj->normalVecs     = new glm::vec3[obj->numVertices];
    obj->texCoords      = new glm::vec2[obj->numVertices];
    memcpy(obj->vertices, &triVert[0], obj->numVertices * sizeof(glm::vec3));
    memcpy(obj->normalVecs, &triNormVecs[0], obj->numVertices * sizeof(glm::vec3));
    memcpy(obj->texCoords, &triTexCoords[0], obj->numVertices * sizeof(glm::vec2));
    return obj;
}

Object::Object() : BaseObject(ObjectType::Object) {
    vao = texture = 0;
    memset(vbo, 0, sizeof(vbo));

    shader = nullptr;

    numVertices = 0;
    vertices = normalVecs = nullptr;
    texCoords = nullptr;
}

void Object::setShader(Shader *shader) { this->shader = shader; }

Shader *Object::getShader() const { return shader; }
