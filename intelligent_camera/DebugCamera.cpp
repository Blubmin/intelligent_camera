#include "DebugCamera.h"

#include <glm\gtc\matrix_transform.hpp>

#include "Globals.h"

using namespace glm;

DebugCamera::DebugCamera()
{
    theta = 0.0f;
    phi = 0.0f;
    pos = vec3(0.0f, 0.0f, 20.0f);
    frontVec = vec3(0.0f, 0.0f, -1.0f);
    upVec = vec3(0.0f, 1.0f, 0.0f);
    speed = 10;
    rotationSpeed = 0.1;
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::update(double timeElapsed)
{
    if (keys[GLFW_KEY_W] == GLFW_PRESS || keys[GLFW_KEY_W] == GLFW_REPEAT)
    {
        pos += frontVec * speed * (float) timeElapsed;
    }
    if (keys[GLFW_KEY_S] == GLFW_PRESS || keys[GLFW_KEY_S] == GLFW_REPEAT)
    {
        pos -= frontVec * speed * (float)timeElapsed;
    }
    if (keys[GLFW_KEY_D] == GLFW_PRESS || keys[GLFW_KEY_D] == GLFW_REPEAT)
    {
        pos += normalize(cross(frontVec, upVec)) * speed * (float)timeElapsed;
    }
    if (keys[GLFW_KEY_A] == GLFW_PRESS || keys[GLFW_KEY_A] == GLFW_REPEAT)
    {
        pos -= normalize(cross(frontVec, upVec)) * speed * (float)timeElapsed;
    }
    if (keys[GLFW_KEY_Q] == GLFW_PRESS || keys[GLFW_KEY_Q] == GLFW_REPEAT)
    {
        pos += upVec * speed * (float)timeElapsed;
    }
    if (keys[GLFW_KEY_E] == GLFW_PRESS || keys[GLFW_KEY_E] == GLFW_REPEAT)
    {
        pos -= upVec * speed * (float)timeElapsed;
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
    return lookAt(pos, pos + frontVec, upVec);
}