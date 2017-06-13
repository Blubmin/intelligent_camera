#include "SelectionSystem.h"

#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include <engine_base\Component.h>
#include <engine_base\Entity.h>
#include <engine_base\GLSL.h>
#include <engine_base\Input.h>
#include <engine_base\Position.h>
#include <engine_base\Scene.h>

#include "IndexRenderer.h"

using namespace glm;
using namespace std;

SelectionSystem::SelectionSystem() {
    _child = new IndexRenderer();

    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    

    GLuint color;
    glGenTextures(1, &color);
    glBindTexture(GL_TEXTURE_2D, color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

    

    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _tex, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


SelectionSystem::~SelectionSystem() {}

void SelectionSystem::update(float time_elapsed, Scene * scene, ICamera* cam) {
    if (Input::button_pressed_down(GLFW_MOUSE_BUTTON_LEFT)) {
        GLint framebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        GLSL::check_gl_error("Fbo");
        GLSL::check_gl_error("Clear stencil");
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLSL::check_gl_error("Clear all");
        glClear(GL_STENCIL_BUFFER_BIT);
        GLSL::check_gl_error("Clear stencil");
        _child->draw(scene, cam);
        GLSL::check_gl_error("Draw");
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        GLSL::check_gl_error("Rebind");

        vec2 pos = Input::cursor_pos();
        GLuint index;
        glReadPixels(pos.x, 720 - pos.y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
        GLSL::check_gl_error("Read");
        if (index > 0) {
            _selected = scene->entities()[index - 1];
            Position* pos = (Position*)(_selected->getComponent(COMPONENT_POSITION));
            pos->position += vec3(0, 1, 0);
        }
    }
}
