#include "RenderingSystem.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "GLSL.h"
#include "Position.h"
#include "World.h"

using namespace glm;
using namespace GLSL;
using namespace std;

RenderingSystem::RenderingSystem()
{
    this->mask = COMPONENT_MODEL | COMPONENT_POSITION;
    this->phong = Program("phong_vert.glsl", "phong_frag.glsl");
    this->projection = perspective((float)radians(45.0), 640.0f / 480.0f, 0.1f, 800.f);
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::render(shared_ptr<World> world)
{
    glUseProgram(this->phong.prog);

    //vertex attributes
    int vertex = this->phong.getAttributeHandle("aPosition");

    mat4 viewMat = world->camera.getViewMatrix();
    glUniformMatrix4fv(this->phong.getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(viewMat));

    glUniformMatrix4fv(this->phong.getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(this->projection));    

    for (int i = 0; i < world->entities.size(); i++)
    {
        shared_ptr<Entity> entity = world->entities[i];
        if (entity->mask & this->mask != this->mask)
            continue;

        shared_ptr<Position> pos = dynamic_pointer_cast<Position>(entity->getComponent(COMPONENT_POSITION));
        glUniformMatrix4fv(this->phong.getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(translate(mat4(), pos->position)));

        shared_ptr<Model> model = dynamic_pointer_cast<Model>(entity->getComponent(COMPONENT_MODEL));
        for (int j = 0; j < model->meshes.size(); j++)
        {
            Mesh mesh = model->meshes.at(i);
            glBindVertexArray(mesh.VAO);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IND);
            glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }
    glUseProgram(0);
}
