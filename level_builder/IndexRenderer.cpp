#include "IndexRenderer.h"

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

using namespace glm;

IndexRenderer::IndexRenderer() :
    IRenderer(COMPONENT_MODEL | COMPONENT_POSITION | COMPONENT_ROTATION | COMPONENT_SCALE) {
    _prog = new Program("flat.vert", "flat.frag");
}

IndexRenderer::~IndexRenderer() {
    delete _prog;
}

glm::mat4 IndexRenderer::create_model_matrix(Entity* entity) {
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

void IndexRenderer::draw(Scene * scene, ICamera* cam) {
    glUseProgram(_prog->prog);

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glUniformMatrix4fv(_prog->uniform("uViewMatrix"), 1, GL_FALSE, value_ptr(cam->getViewMatrix()));
    glUniformMatrix4fv(_prog->uniform("uProjMatrix"), 1, GL_FALSE, value_ptr(cam->getProjectionMatrix()));
    int i = 0;
    for (auto e : scene->entities()) {
        if (!e->check_mask(_mask)) continue;

        glStencilFunc(GL_ALWAYS, i + 1, -1);
        glUniformMatrix4fv(_prog->uniform("uModelMatrix"), 1, GL_FALSE, value_ptr(create_model_matrix(e)));

        Model* model = (Model*)e->getComponent(COMPONENT_MODEL);
        
        for (auto mesh : model->meshes) {

            glBindVertexArray(mesh.VAO);
            glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
        i++;
    }

    glUseProgram(0);
}
