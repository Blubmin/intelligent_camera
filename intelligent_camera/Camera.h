#pragma once

#include <glm\glm.hpp>

class Camera
{
public:
    Camera();
    ~Camera();

    virtual void update(double timeElapsed) = 0;
    virtual glm::mat4 getViewMatrix() = 0;
};

