#pragma once

#include "baseobject.hpp"

struct Camera : BaseObject {
private:
    float fov = 45.0f, aspectRatio = 4.0f / 3.0f, nearClip = 0.1f, farClip = 100.0f;
public:
    Camera();
    glm::mat4 getView() const;
    glm::mat4 getProj() const;
};