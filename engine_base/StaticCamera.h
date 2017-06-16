#pragma once
#include "ICamera.h"

#include <glm\glm.hpp>

class StaticCamera :
    public ICamera {

    glm::vec3 _pos;
    glm::vec3 _look_at;
    float _aspect;

public:
    StaticCamera(glm::vec3 pos, glm::vec3 look_at, float aspect);
    ~StaticCamera();

    void update(float time_elapsed);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
};

