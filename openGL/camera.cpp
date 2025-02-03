#include "camera.hpp"

Camera::Camera() { }

glm::mat4 Camera::getView() const {
    glm::mat4 rotMat = glm::mat4_cast(rotationQuat);
    glm::vec3 forward = glm::normalize(glm::vec3(rotMat * glm::vec4(0, 0, -1.0f, 1.0f))),
        up = glm::normalize(glm::vec3(rotMat * glm::vec4(0, 1.0f, 0, 1.0f)));
    return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::getProj() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}