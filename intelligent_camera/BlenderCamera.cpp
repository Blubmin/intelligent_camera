#include "BlenderCamera.h"

#include <iostream>

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>

#include <engine_base\Input.h>

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
    this->pos += forward * this->zoomSpeed * (float) timeElapsed * (float) Input::scroll().y;

    vec2 mouse_change = Input::cursor_change();

    if (length(mouse_change) < FLT_EPSILON) return;

    if (!Input::button_pressed_down(GLFW_MOUSE_BUTTON_MIDDLE)) return;

    if (Input::key_pressed_down(GLFW_KEY_LEFT_SHIFT)) {
        vec3 shift_vec = normalize(cross(this->pos - this->lookAtPt, vec3(0, 1, 0)));
<<<<<<< Updated upstream
        vec3 shift = shift_vec * (float)(mouse_x_diff * timeElapsed);
        vec3 up_shift_vec = normalize(cross(this->pos - this->lookAtPt, shift_vec));
        vec3 up_shift = up_shift_vec * (float)(-mouse_y_diff * timeElapsed);
=======
        vec3 shift = shift_vec * (float)(mouse_change.x * timeElapsed) * moveSpeed;
        vec3 up_shift_vec = normalize(cross(this->pos - this->lookAtPt, shift_vec));
        vec3 up_shift = up_shift_vec * (float)(-mouse_change.y * timeElapsed) * moveSpeed;
>>>>>>> Stashed changes
        this->pos += shift;
        this->lookAtPt += shift;
        this->pos += up_shift;
        this->lookAtPt += up_shift;
    }
    else {
        this->pos -= this->lookAtPt;
        this->pos = rotate(mat4(), (float)(-mouse_change.x * timeElapsed), vec3(0, 1, 0)) * vec4(this->pos, 1);
        this->pos = rotate(mat4(), (float)(mouse_change.y * timeElapsed), cross(this->pos, vec3(0, 1, 0))) * vec4(this->pos, 1);
        this->pos += this->lookAtPt;
    }
}

mat4 BlenderCamera::getViewMatrix()
{
    return lookAt(pos, lookAtPt, vec3(0, 1, 0));
}
