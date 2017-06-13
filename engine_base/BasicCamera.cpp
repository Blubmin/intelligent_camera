#include "BasicCamera.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Input.h"

using namespace glm;

BasicCamera::BasicCamera() : BasicCamera(vec3(0, 0, 0)) {
    
}

BasicCamera::BasicCamera(vec3 starting_pos) {
    _pos = starting_pos;
    _front_vec = glm::vec3(0, 0, 1);
    _theta = degrees(atan(_front_vec.x / _front_vec.z));
    _phi = degrees(atan(_front_vec.y / length(vec2(_front_vec.x, _front_vec.z))));
    _speed = 40;
    _rotation_speed = 0.1;
}

BasicCamera::~BasicCamera() {}

void BasicCamera::update(float time_elapsed) {
    vec3 up_vec = vec3(0, 1, 0);
    if (Input::key_pressed_down(GLFW_KEY_W)) {
        _pos += _front_vec * _speed * time_elapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_S)) {
        _pos -= _front_vec * _speed * time_elapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_D)) {
        _pos += normalize(cross(_front_vec, up_vec)) * _speed * time_elapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_A)) {
        _pos -= normalize(cross(_front_vec, up_vec)) * _speed * time_elapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_Q)) {
        _pos += up_vec * _speed * time_elapsed;
    }
    if (Input::key_pressed_down(GLFW_KEY_E)) {
        _pos -= up_vec * _speed * time_elapsed;
    }

    vec2 mouse_change = Input::cursor_change();

    if (length(mouse_change) < FLT_EPSILON) return;

    _theta += mouse_change.x * _rotation_speed;
    _phi -= mouse_change.y * _rotation_speed;

    if (_phi > 89.0f)
        _phi = 89.0f;
    if (_phi < -89.0f)
        _phi = -89.0f;

    vec3 front;
    front.x = cos(radians(_theta)) * cos(radians(_phi));
    front.y = sin(radians(_phi));
    front.z = sin(radians(_theta)) * cos(radians(_phi));
    _front_vec = normalize(front);
}

glm::mat4 BasicCamera::getViewMatrix() {
    return glm::lookAt(_pos, _pos + _front_vec, vec3(0, 1, 0));
}

glm::mat4 BasicCamera::getProjectionMatrix() {
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    return glm::perspective(radians(45.f), width / (float) height, 0.01f, 1000.f);
}