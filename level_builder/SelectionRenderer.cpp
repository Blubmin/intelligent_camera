#include "SelectionRenderer.h"

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <engine_base\Component.h>
#include <engine_base\Entity.h>
#include <engine_base\GLSL.h>
#include <engine_base\ICamera.h>
#include <engine_base\Model.h>
#include <engine_base\Position.h>
#include <engine_base\Program.h>
#include <engine_base\Rotation.h>
#include <engine_base\Scale.h>
#include <engine_base\Scene.h>

#include <shaders/shaders.h>

#include "Selected.h"

using namespace glm;
using namespace std;

SelectionRenderer::SelectionRenderer() : IRenderer(COMPONENT_MODEL | 
                                                   COMPONENT_POSITION | 
                                                   COMPONENT_ROTATION | 
                                                   COMPONENT_SCALE | 
                                                   COMPONENT_SELECTED) {
    _prog = new Program(flat_vert_size, flat_vert, flat_frag_size, flat_frag);
    _prog_outline = new Program(outline_vert_size, outline_vert, outline_frag_size, outline_frag);
}


SelectionRenderer::~SelectionRenderer() {}

glm::mat4 SelectionRenderer::create_outline_model_matrix(Entity* entity) {
    Position* pos = (Position*)entity->getComponent(COMPONENT_POSITION);
    Rotation* rot = (Rotation*)entity->getComponent(COMPONENT_ROTATION);
    Scale* scale = (Scale*)entity->getComponent(COMPONENT_SCALE);

    mat4 dest = mat4();

    dest = glm::translate(dest, vec3(pos->position));

    dest = glm::scale(dest, vec3(scale->x + .1, scale->y + .1, scale->z + .1));

    dest = glm::rotate(dest, rot->phi, vec3(1, 0, 0));
    dest = glm::rotate(dest, rot->theta, vec3(0, 1, 0));
    dest = glm::rotate(dest, rot->psi, vec3(0, 0, 1));

    return dest;
}

glm::mat4 SelectionRenderer::create_model_matrix(Entity* entity) {
    Position* pos = (Position*)entity->getComponent(COMPONENT_POSITION);
    Rotation* rot = (Rotation*)entity->getComponent(COMPONENT_ROTATION);
    Scale* scale = (Scale*)entity->getComponent(COMPONENT_SCALE);

    mat4 dest = mat4();

    dest = glm::translate(dest, vec3(pos->position));

    dest = glm::rotate(dest, rot->phi, vec3(1, 0, 0));
    dest = glm::rotate(dest, rot->theta, vec3(0, 1, 0));
    dest = glm::rotate(dest, rot->psi, vec3(0, 0, 1));

    dest = glm::scale(dest, vec3(scale->x, scale->y, scale->z));

    return dest;
}

void SelectionRenderer::draw(Scene * scene, ICamera * cam) {

    glUseProgram(_prog->prog);

    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glUniformMatrix4fv(_prog->uniform("uViewMatrix"), 1, GL_FALSE, value_ptr(cam->getViewMatrix()));
    glUniformMatrix4fv(_prog->uniform("uProjMatrix"), 1, GL_FALSE, value_ptr(cam->getProjectionMatrix()));
    glUniform3fv(_prog->uniform("uDirLight"), 1, value_ptr(scene->dir_light()));

    Entity* select = nullptr;

    for (auto e : scene->entities()) {
        if (!e->check_mask(_mask)) continue;

        glUniformMatrix4fv(_prog->uniform("uModelMatrix"), 1, GL_FALSE, value_ptr(create_model_matrix(e)));

        Model* model = (Model*) e->getComponent(COMPONENT_MODEL);
        Selected* selected = (Selected*)e->getComponent(COMPONENT_SELECTED);

        if (selected->selected()) select = e;
        glStencilFunc(GL_ALWAYS, selected->selected() ? 1 : 0, 0xFF);

        for (auto m : model->meshes) {

            glUniform3fv(_prog->uniform("uDiffuseColor"), 1, value_ptr(model->materials[m.materialIdx].diffuse));

            glBindVertexArray(m.VAO);
            glDrawElements(GL_TRIANGLES, m.indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    if (select != nullptr) {
        glUseProgram(_prog_outline->prog);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(5);

        glUniformMatrix4fv(_prog_outline->uniform("uViewMatrix"), 1, GL_FALSE, value_ptr(cam->getViewMatrix()));
        glUniformMatrix4fv(_prog_outline->uniform("uProjMatrix"), 1, GL_FALSE, value_ptr(cam->getProjectionMatrix()));
        glUniformMatrix4fv(_prog_outline->uniform("uModelMatrix"), 1, GL_FALSE, value_ptr(create_model_matrix(select)));
        glUniform3f(_prog_outline->uniform("uDiffuseColor"), 1.0f, 1.0f, 0.0f); // Yellow

        Model* model = (Model*)select->getComponent(COMPONENT_MODEL);

        for (auto m : model->meshes) {
            glBindVertexArray(m.VAO);
            glDrawElements(GL_TRIANGLES, m.indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glStencilMask(0xFF);
    }

    glDisable(GL_STENCIL_TEST);
    glUseProgram(0);
}
