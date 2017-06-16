#pragma once

#include <engine_base\ICamera.h>

#include <glm/glm.hpp>

class BlenderCamera : public ICamera
{

    glm::vec3 _pos;

public:
    BlenderCamera();
    ~BlenderCamera();
    void update(float timeElapsed);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    glm::vec3 lookAtPt;
    float zoomSpeed;
    float moveSpeed;
};

