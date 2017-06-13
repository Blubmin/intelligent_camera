#include "FlatRenderer.h"

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Component.h"
#include "Entity.h"
#include "GLSL.h"
#include "ICamera.h"
#include "Model.h"
#include "Position.h"
#include "Program.h"
#include "Rotation.h"
#include "Scale.h"
#include "Scene.h"

using namespace glm;

FlatRenderer::FlatRenderer() :
    IRenderer(COMPONENT_MODEL | COMPONENT_POSITION | COMPONENT_ROTATION | COMPONENT_SCALE)
{
    _prog = new Program("flat.vert", "flat.frag");
}

FlatRenderer::~FlatRenderer() {
    delete _prog;
}

glm::mat4 FlatRenderer::create_model_matrix(Entity* entity) {    
    Position* pos = (Position*)entity->getComponent(COMPONENT_POSITION);
    Rotation* rot = (Rotation*)entity->getComponent(COMPONENT_ROTATION);
    Scale* scale = (Scale*)entity->getComponent(COMPONENT_SCALE);

    mat4 dest = mat4();

    dest = glm::translate(dest, vec3(pos->position));

    dest = glm::scale(dest, vec3(scale->x, scale->y, scale->z));

    dest = glm::rotate(dest, rot->phi, vec3(1, 0, 0));
    dest = glm::rotate(dest, rot->theta, vec3(0, 1, 0));
    dest = glm::rotate(dest, rot->psi, vec3(0, 0, 1));

    return dest;
}

void FlatRenderer::draw(Scene * scene, ICamera* cam) {
    glUseProgram(_prog->prog);

    glUniformMatrix4fv(_prog->uniform("uViewMatrix"), 1, GL_FALSE, value_ptr(cam->getViewMatrix()));
    glUniformMatrix4fv(_prog->uniform("uProjMatrix"), 1, GL_FALSE, value_ptr(cam->getProjectionMatrix()));
    glUniform3fv(_prog->uniform("uDirLight"), 1, value_ptr(scene->dir_light()));

    for (auto e : scene->entities()) {
        if (!e->check_mask(_mask)) continue;

        glUniformMatrix4fv(_prog->uniform("uModelMatrix"), 1, GL_FALSE, value_ptr(create_model_matrix(e)));

        Model* model = (Model*) e->getComponent(COMPONENT_MODEL);
        for (auto mesh : model->meshes) {

            glUniform3fv(_prog->uniform("uDiffuseColor"), 1, value_ptr(model->materials[mesh.materialIdx].diffuse));

            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    glUseProgram(0);
}
