#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>
#include <vector>

enum class ObjectType {
    Empty, Camera, Object3D, Object2D, Light
};

enum class ObjectVisibility {
    Disable, Unvisible, Visible
};

struct BaseObject {
protected:
    glm::vec3 position, scaler;
    glm::quat rotationQuat;

    std::vector<BaseObject *> children;
    BaseObject *parent;
    size_t parentIndex;

    ObjectType type;
    ObjectVisibility visibility;

public:
    BaseObject(ObjectType type = ObjectType::Empty, ObjectVisibility visibility = ObjectVisibility::Disable);
    
    void translateGlobal(const glm::vec3 &vec);
    void translateLocal(const glm::vec3 &vec);
    void rotate(const float angle, const glm::vec3 &axis);
    void rotateAround(const glm::vec3 &pivot, const glm::vec3 &axis, float angle);
    void lookAt(glm::vec3 pos, glm::vec3 up);
    void scale(glm::vec3 vec);

    glm::vec3 right() const;
    glm::vec3 up() const;
    glm::vec3 forward() const;

    glm::vec3 getRotateEuler() const;
    glm::vec3 getPosition() const;
    glm::vec3 getScale() const;

    void setPosition(const glm::vec3 &vec);
    void setRotation(const glm::vec3 &euler);
    void setScale(const glm::vec3 &scale);

    glm::mat4 getModelMat() const;

    BaseObject *getParent() const;
    void setParent(BaseObject *parent);

    ObjectVisibility getVisibility() const;
    void setVisibility(ObjectVisibility visibility);

    std::vector<BaseObject *> &getChildren();
};