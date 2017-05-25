#include "BlenderCamera.h"

#include <iostream>

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include "Globals.h"

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

void BlenderCamera::update(double timeElapsed)
{
    vec3 forward = this->lookAtPt - _pos;
    _pos += forward * this->zoomSpeed * (float) timeElapsed * (float) scroll;

    static double prev_mouse_x = 0;
    static double prev_mouse_y = 0;

    if (prev_mouse_x == mouse_x && prev_mouse_y == mouse_y) return;

    if (mouse_buttons[GLFW_MOUSE_BUTTON_MIDDLE] != GLFW_PRESS) return;

    if (mods & GLFW_MOD_SHIFT) {
        vec3 shift_vec = normalize(cross(_pos - this->lookAtPt, vec3(0, 1, 0)));
        vec3 shift = shift_vec * (float)(mouse_x_diff * timeElapsed) * moveSpeed;
        vec3 up_shift_vec = normalize(cross(_pos - this->lookAtPt, shift_vec));
        vec3 up_shift = up_shift_vec * (float)(-mouse_y_diff * timeElapsed) * moveSpeed;
        _pos += shift;
        this->lookAtPt += shift;
        _pos += up_shift;
        this->lookAtPt += up_shift;
    }
    else {
        _pos -= this->lookAtPt;
        _pos = rotate(mat4(), (float)(-mouse_x_diff * timeElapsed), vec3(0, 1, 0)) * vec4(_pos, 1);
        _pos = rotate(mat4(), (float)(mouse_y_diff * timeElapsed), cross(_pos, vec3(0, 1, 0))) * vec4(_pos, 1);
        _pos += this->lookAtPt;
    }

    prev_mouse_x = mouse_x;
    prev_mouse_y = mouse_y;
}

mat4 BlenderCamera::getViewMatrix()
{
    return lookAt(_pos, lookAtPt, vec3(0, 1, 0));
}
