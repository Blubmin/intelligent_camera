#include "BlenderCamera.h"

#include <iostream>

#include <glm\gtc\matrix_transform.hpp>

#include "Globals.h"

using namespace glm;

BlenderCamera::BlenderCamera()
{
    this->pos = vec3(20, 20, 20);
    this->lookAtPt = vec3(0, 0, 0);
    this->zoomSpeed = 10;
}


BlenderCamera::~BlenderCamera()
{
}

void BlenderCamera::update(double timeElapsed)
{
    vec3 forward = this->lookAtPt - this->pos;
    this->pos += forward * this->zoomSpeed * (float) timeElapsed * (float) scroll;

    static double prev_mouse_x = 0;
    static double prev_mouse_y = 0;

    if (prev_mouse_x == mouse_x && prev_mouse_y == mouse_y) return;

    if (mouse_buttons[GLFW_MOUSE_BUTTON_MIDDLE] != GLFW_PRESS) return;

    if (mods & GLFW_MOD_SHIFT) {
        vec3 shift_vec = normalize(cross(this->pos - this->lookAtPt, vec3(0, 1, 0)));
        vec3 shift = shift_vec * (float)(mouse_x_diff * timeElapsed);
        vec3 up_shift_vec = normalize(cross(this->pos - this->lookAtPt, shift_vec));
        vec3 up_shift = up_shift_vec * (float)(-mouse_y_diff * timeElapsed);
        this->pos += shift;
        this->lookAtPt += shift;
        this->pos += up_shift;
        this->lookAtPt += up_shift;
    }
    else {
        this->pos -= this->lookAtPt;
        this->pos = rotate(mat4(), (float)(-mouse_x_diff * timeElapsed), vec3(0, 1, 0)) * vec4(this->pos, 1);
        this->pos = rotate(mat4(), (float)(mouse_y_diff * timeElapsed), cross(this->pos, vec3(0, 1, 0))) * vec4(this->pos, 1);
        this->pos += this->lookAtPt;
    }

    prev_mouse_x = mouse_x;
    prev_mouse_y = mouse_y;
}

mat4 BlenderCamera::getViewMatrix()
{
    return lookAt(pos, lookAtPt, vec3(0, 1, 0));
}
