#include "SelectionSystem.h"

#include <iostream>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <imgui\imgui.h>

#include <engine_base\Component.h>
#include <engine_base\Entity.h>
#include <engine_base\GLSL.h>
#include <engine_base\Input.h>
#include <engine_base\Position.h>
#include <engine_base\Rotation.h>
#include <engine_base\Scale.h>
#include <engine_base\Scene.h>
#include <engine_base\TextureRenderer.h>

#include "IndexRenderer.h"
#include "Selected.h"

using namespace glm;
using namespace std;

SelectionSystem::SelectionSystem() {
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    _child = new TextureRenderer(new IndexRenderer(), width, height);
    _selected = nullptr;
}


SelectionSystem::~SelectionSystem() {}

void SelectionSystem::update(float time_elapsed, Scene * scene, ICamera* cam) {
    if (_selected != nullptr) {
        Position* pos = (Position*)(_selected->getComponent(COMPONENT_POSITION));
        Rotation* rot = (Rotation*)(_selected->getComponent(COMPONENT_ROTATION));
        Scale* scale = (Scale*)(_selected->getComponent(COMPONENT_SCALE));

        pos->position = _pos;
        rot->phi = _rot.x;
        rot->theta = _rot.y;
        rot->psi = _rot.z;
        scale->x = _scale.x;
        scale->y = _scale.y;
        scale->z = _scale.z;
    }


    if (Input::button_pressed(GLFW_MOUSE_BUTTON_LEFT) && !ImGui::IsMouseHoveringAnyWindow()) {
       
        _child->draw(scene, cam);
        glBindFramebuffer(GL_FRAMEBUFFER, _child->fbo());

        vec2 pos = Input::cursor_pos();
        GLuint index;
        glReadPixels(pos.x, 720 - pos.y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

        if (index > 0) {
            select(scene->entities()[index - 1]);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void SelectionSystem::select(Entity * entity) {
    if (_selected != nullptr) {
        Selected* sel = (Selected*)(_selected->getComponent(COMPONENT_SELECTED));
        sel->deselect();
    }
    _selected = entity;
    Selected* sel = (Selected*)(_selected->getComponent(COMPONENT_SELECTED));
    Position* pos = (Position*)(_selected->getComponent(COMPONENT_POSITION));
    Rotation* rot = (Rotation*)(_selected->getComponent(COMPONENT_ROTATION));
    Scale* scale = (Scale*)(_selected->getComponent(COMPONENT_SCALE));
    sel->select();
    _pos = pos->position;
    _rot = vec3(rot->phi, rot->theta, rot->psi);
    _scale = vec3(scale->x, scale->y, scale->z);
}

bool SelectionSystem::selected() {
    return _selected != nullptr;
}

glm::vec3 * SelectionSystem::pos() {
    return &_pos;
}

glm::vec3 * SelectionSystem::rot() {
    return &_rot;
}

glm::vec3 * SelectionSystem::scale() {
    return &_scale;
}