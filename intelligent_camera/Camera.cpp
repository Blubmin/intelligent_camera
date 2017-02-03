#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

using namespace glm;

Camera::Camera()
{
    theta = 0.0f;
    phi = 0.0f;
    pos = vec3(0.0f, 5.0f, 5.0f);
    frontVec = vec3(0.0f, 0.0f, -1.0f);
    upVec = vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera()
{
}

mat4 Camera::getViewMatrix()
{
    /*return lookAt(pos, pos + frontVec, upVec);*/
    return lookAt(pos, vec3(0, 0, 0), upVec);
}