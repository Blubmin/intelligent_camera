#pragma once

#include <glm/glm.hpp>

#include "Camera.h"

class BlenderCamera : public Camera
{
public:
    BlenderCamera();
    ~BlenderCamera();
    void update(double timeElapsed);
    glm::mat4 getViewMatrix();

    glm::vec3 lookAtPt;
    float zoomSpeed;
    float moveSpeed;
};

