#pragma once

#include <glm\glm.hpp>

class Camera
{
public:
    Camera();
    ~Camera();

    glm::mat4 getViewMatrix();

    glm::vec3 pos;
    glm::vec3 upVec;
    glm::vec3 frontVec;
    float theta;
    float phi;
};

