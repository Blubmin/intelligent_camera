#pragma once
#include "ICamera.h"

#include <glm\glm.hpp>

class BasicCamera : public ICamera {
    glm::vec3 _pos;
    glm::vec3 _front_vec;
    float _theta;
    float _phi;
    float _speed;
    float _rotation_speed;

public:
    BasicCamera();
    BasicCamera(glm::vec3 starting_pos);
    ~BasicCamera();

    void update(float time_elapsed);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
};

