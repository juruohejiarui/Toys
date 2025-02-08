#pragma once

#include "baseobject.hpp"

struct Camera : BaseObject {
private:
    float fov = 30.0f, aspectRatio = 1920.0f / 1080.0f, nearClip = 0.1f, farClip = 100.0f;
public:
    Camera();
    glm::mat4 getView() const;
    glm::mat4 getProj() const;
};