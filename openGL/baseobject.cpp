#include "baseobject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

BaseObject::BaseObject(ObjectType type, ObjectVisibility visibility) {
    position        = glm::vec3(0);
    rotationQuat    = glm::quat(1, 0, 0, 0);
    scaler          = glm::vec3(1.0f);
    this->visibility = visibility;
    this->type = type;
    parent = nullptr;
}

inline void BaseObject::translateGlobal(const glm::vec3 &vec)
{
    this->position += vec;
}

void BaseObject::translateLocal(const glm::vec3 &vec) {
    this->position += glm::vec3(glm::toMat4(this->rotationQuat) * glm::vec4(vec, 0.0f));
}

void BaseObject::rotateGlobal(const glm::vec3 &angle) {
    this->rotationQuat = glm::quat(getRotateEuler() + angle);
}

void BaseObject::rotateLocal(const glm::vec3 &angle) {
    this->rotationQuat = glm::quat(angle) * this->rotationQuat;
}

void BaseObject::rotateAround(const glm::vec3 &pivot, const glm::vec3 &axis, float angle) {
    angle = glm::radians(angle);
    glm::vec4 rel = glm::rotate(glm::mat4(1.0f), angle, glm::normalize(axis)) * glm::vec4(position - pivot, 1.0f);
    position = glm::vec3(rel) + pivot;
    rotationQuat = glm::angleAxis(angle, glm::normalize(axis)) * rotationQuat;
}

void BaseObject::lookAt(glm::vec3 pos, glm::vec3 up) {
    glm::vec3 forward = glm::normalize(pos - position),
        right = glm::normalize(glm::cross(up, forward)),
        newUp = glm::cross(forward, right);
    
    glm::mat4 lookAtMat = glm::mat4(1.0f);
    lookAtMat[0] = glm::vec4(right, 0.0f);
    lookAtMat[1] = glm::vec4(newUp, 0.0f);
    lookAtMat[2] = glm::vec4(forward, 0.0f);

    rotationQuat = glm::quat_cast(lookAtMat);
}

void BaseObject::scale(glm::vec3 vec) { scaler = vec * scaler; }

glm::vec3 BaseObject::getRotateEuler() { return glm::eulerAngles(rotationQuat); }

BaseObject *BaseObject::getParent() const { return parent; }

void BaseObject::setParent(BaseObject *parent) {
    if (this->parent != nullptr) {
        this->parent->getChildren().erase(this->parent->getChildren().begin() + this->parentIndex);
    }
    this->parentIndex = parent->getChildren().size();
    this->parent = parent;
}

ObjectVisibility BaseObject::getVisibility() const { return visibility; }

void BaseObject::setVisibility(ObjectVisibility visibility) { this->visibility = visibility; }

std::vector<BaseObject *> &BaseObject::getChildren() { return children; }
