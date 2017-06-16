#include "BlenderCamera.h"

#include <iostream>

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include <imgui\imgui.h>

#include <engine_base\Input.h>

using namespace glm;

BlenderCamera::BlenderCamera()
{
    _pos = vec3(10, 5, 10);
    this->lookAtPt = vec3(0, 0, 0);
    this->zoomSpeed = 30;
    this->moveSpeed = 5;
}


BlenderCamera::~BlenderCamera()
{
}

void BlenderCamera::update(float timeElapsed)
{
    vec3 forward = this->lookAtPt - _pos;
    float scroll = ImGui::IsMouseHoveringAnyWindow() ? 0 : Input::scroll().y;
    _pos += forward * this->zoomSpeed * (float)timeElapsed * scroll;

    vec2 mouse_change = Input::cursor_change();

    if (length(mouse_change) < FLT_EPSILON) return;

    if (!Input::button_pressed_down(GLFW_MOUSE_BUTTON_MIDDLE)) return;

    if (Input::key_pressed_down(GLFW_KEY_LEFT_SHIFT)) {
        vec3 shift_vec = normalize(cross(_pos - this->lookAtPt, vec3(0, 1, 0)));
        vec3 shift = shift_vec * (float)(mouse_change.x * timeElapsed) * moveSpeed;
        vec3 up_shift_vec = normalize(cross(_pos - this->lookAtPt, shift_vec));
        vec3 up_shift = up_shift_vec * (float)(-mouse_change.y * timeElapsed) * moveSpeed;
        _pos += shift;
        this->lookAtPt += shift;
        _pos += up_shift;
        this->lookAtPt += up_shift;
    }
    else {
        _pos -= this->lookAtPt;
        _pos = rotate(mat4(), (float)(-mouse_change.x * timeElapsed), vec3(0, 1, 0)) * vec4(_pos, 1);
        _pos = rotate(mat4(), (float)(mouse_change.y * timeElapsed), cross(_pos, vec3(0, 1, 0))) * vec4(_pos, 1);
        _pos += this->lookAtPt;
    }
}

mat4 BlenderCamera::getViewMatrix()
{
    return lookAt(_pos, lookAtPt, vec3(0, 1, 0));
}

mat4 BlenderCamera::getProjectionMatrix() {
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    return perspective(45.0f, width / (float) height, 0.01f, 1000.0f);
}