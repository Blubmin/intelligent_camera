#include "StaticCamera.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace glm;

StaticCamera::StaticCamera(glm::vec3 pos, glm::vec3 look_at, float aspect) {
    _pos = pos;
    _look_at = look_at;
    _aspect = aspect;
}

StaticCamera::~StaticCamera() {}

void StaticCamera::update(float time_elapsed) {}

glm::mat4 StaticCamera::getViewMatrix() {
    return lookAt(_pos, _look_at, vec3(0, 1, 0));
}

glm::mat4 StaticCamera::getProjectionMatrix() {
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    return perspective(45.f, _aspect, 0.01f, 1000.0f);
}
