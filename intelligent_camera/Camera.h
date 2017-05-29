#pragma once

#include <glm\glm.hpp>

class Camera
{
protected:
    glm::vec3 _pos;

public:
    Camera();
    ~Camera();

    virtual void update(double timeElapsed) = 0;
    virtual glm::mat4 getViewMatrix() = 0;
    glm::vec3 pos();
};

