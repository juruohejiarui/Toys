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

void BaseObject::translateGlobal(const glm::vec3 &vec) {
    this->position += vec;
}

void BaseObject::translateLocal(const glm::vec3 &vec) {
    this->position += this->rotationQuat * vec;
}

void BaseObject::rotate(float angle, const glm::vec3 &axis) {
    this->rotationQuat = glm::normalize(glm::angleAxis(glm::radians(angle), axis) * rotationQuat);
}

void BaseObject::rotateAround(const glm::vec3 &pivot, const glm::vec3 &axis, float angle) {
    glm::quat rot = glm::angleAxis(glm::radians(angle), axis);
    position = pivot + rot * (position - pivot);
    rotationQuat = rot * rotationQuat;
}

void BaseObject::lookAt(glm::vec3 pos, glm::vec3 up) {
    glm::vec3 direction = glm::normalize(pos - position);
    glm::vec3 right = glm::normalize(glm::cross(up, direction));
    glm::vec3 correctedUp = glm::cross(direction, right);
    glm::mat3 lookAtMatrix(right, correctedUp, -direction);
    rotationQuat = glm::quat_cast(lookAtMatrix);
}

void BaseObject::scale(glm::vec3 vec) { scaler = vec * scaler; }

glm::vec3 BaseObject::right() const { return rotationQuat * glm::vec3(1, 0, 0); }
glm::vec3 BaseObject::up() const { return rotationQuat * glm::vec3(0, 1, 0); }
glm::vec3 BaseObject::forward() const { return rotationQuat * glm::vec3(0, 0, -1); }

glm::vec3 BaseObject::getPosition() const { return this->position; }
glm::vec3 BaseObject::getRotateEuler() const { return glm::eulerAngles(rotationQuat); }
glm::vec3 BaseObject::getScale() const { return this->scaler; }

void BaseObject::setPosition(const glm::vec3 &vec) { this->position = vec; }
void BaseObject::setRotation(const glm::vec3 &euler) { this->rotationQuat = glm::quat(euler); }
void BaseObject::setScale(const glm::vec3 &scale) { this->scaler = scale; }

glm::mat4 BaseObject::getModelMat() const {
    glm::mat4 
        tran = glm::translate(glm::mat4(1.0f), this->position),
        rota = glm::mat4_cast(this->rotationQuat),
        scal = glm::scale(glm::mat4(1.0f), this->scaler);
    return tran * rota * scal;
}

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
