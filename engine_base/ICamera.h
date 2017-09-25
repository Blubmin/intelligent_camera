#pragma once

#include <glm/glm.hpp>

class ICamera {
public:
    ICamera();
    ~ICamera();

    virtual void update(float time_elapsed) = 0;
    virtual glm::mat4 getViewMatrix() = 0;
    virtual glm::mat4 getProjectionMatrix() = 0;
};

