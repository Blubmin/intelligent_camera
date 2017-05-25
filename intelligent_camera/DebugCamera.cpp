#include "DebugCamera.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Globals.h"

using namespace glm;

DebugCamera::DebugCamera() {}

DebugCamera::DebugCamera(vec3 starting_pos)
{
    _pos = starting_pos;
    frontVec = -normalize(starting_pos);
    theta = degrees(atan(frontVec.x / frontVec.z));
    phi = degrees(atan(frontVec.y / length(vec2(frontVec.x, frontVec.z))));
    upVec = vec3(0.0f, 1.0f, 0.0f);
    speed = 40;
    rotationSpeed = 0.1;
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::update(double timeElapsed)
{
    if (keys[GLFW_KEY_W] == GLFW_PRESS || keys[GLFW_KEY_W] == GLFW_REPEAT)
    {
        _pos += frontVec * speed * (float) timeElapsed;
    }
    if (keys[GLFW_KEY_S] == GLFW_PRESS || keys[GLFW_KEY_S] == GLFW_REPEAT)
    {
        _pos -= frontVec * speed * (float)timeElapsed;
    }
    if (keys[GLFW_KEY_D] == GLFW_PRESS || keys[GLFW_KEY_D] == GLFW_REPEAT)
    {
        _pos += normalize(cross(frontVec, upVec)) * speed * (float)timeElapsed;
    }
    if (keys[GLFW_KEY_A] == GLFW_PRESS || keys[GLFW_KEY_A] == GLFW_REPEAT)
    {
        _pos -= normalize(cross(frontVec, upVec)) * speed * (float)timeElapsed;
    }
    if (keys[GLFW_KEY_Q] == GLFW_PRESS || keys[GLFW_KEY_Q] == GLFW_REPEAT)
    {
        _pos += upVec * speed * (float)timeElapsed;
    }
    if (keys[GLFW_KEY_E] == GLFW_PRESS || keys[GLFW_KEY_E] == GLFW_REPEAT)
    {
        _pos -= upVec * speed * (float)timeElapsed;
    }

    static double prev_mouse_x = 0;
    static double prev_mouse_y = 0;

    if (prev_mouse_x == mouse_x && prev_mouse_y == mouse_y) return;
    theta += mouse_x_diff * rotationSpeed;
    phi -= mouse_y_diff * rotationSpeed;
    prev_mouse_x = mouse_x;
    prev_mouse_y = mouse_y;

    if (phi > 89.0f)
        phi = 89.0f;
    if (phi < -89.0f)
        phi = -89.0f;

    vec3 front;
    front.x = cos(radians(theta)) * cos(radians(phi));
    front.y = sin(radians(phi));
    front.z = sin(radians(theta)) * cos(radians(phi));
    frontVec = normalize(front);
}

mat4 DebugCamera::getViewMatrix()
{
    return lookAt(_pos, _pos + frontVec, upVec);
}