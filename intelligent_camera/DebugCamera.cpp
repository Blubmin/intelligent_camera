#include "DebugCamera.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include <engine_base\Input.h>

using namespace glm;

DebugCamera::DebugCamera() {}

DebugCamera::DebugCamera(vec3 starting_pos)
{
    pos = starting_pos;
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
    if (Input::key_pressed_down(GLFW_KEY_W)) {
        pos += frontVec * speed * (float) timeElapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_S)) {
        pos -= frontVec * speed * (float)timeElapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_D)) {
        pos += normalize(cross(frontVec, upVec)) * speed * (float)timeElapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_A)) {
        pos -= normalize(cross(frontVec, upVec)) * speed * (float)timeElapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_Q)) {
        pos += upVec * speed * (float)timeElapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_E)) {
        pos -= upVec * speed * (float)timeElapsed;
    }

    vec2 mouse_change = Input::cursor_change();

    if (length(mouse_change) < FLT_EPSILON) return;

    theta += mouse_change.x * rotationSpeed;
    phi -= mouse_change.y * rotationSpeed;

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