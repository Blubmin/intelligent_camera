#pragma once

#include <glm\glm.hpp>

#include "Camera.h"

class DebugCamera : public Camera
{
public:
    DebugCamera();
    DebugCamera(glm::vec3 starting_pos);
    ~DebugCamera();

    void update(double elapsedTime);
    glm::mat4 getViewMatrix();

    glm::vec3 upVec;
    glm::vec3 frontVec;
    float theta;
    float phi;
    float speed;
    float rotationSpeed;
};

