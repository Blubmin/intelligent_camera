#pragma once

#include <glm\glm.hpp>

#include "Camera.h"

class DebugCamera : public Camera
{
public:
    DebugCamera();
    ~DebugCamera();

    void update(double elapsedTime);
    glm::mat4 getViewMatrix();

    glm::vec3 pos;
    glm::vec3 upVec;
    glm::vec3 frontVec;
    float theta;
    float phi;
    float speed;
    float rotationSpeed;
};

