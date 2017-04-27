#include "RenderingSystem.h"

#include <iostream>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "GLSL.h"
#include "Player.h"
#include "Position.h"
#include "Rotation.h"
#include "Scale.h"
#include "World.h"

using namespace glm;
using namespace GLSL;
using namespace std;

RenderingSystem::RenderingSystem()
{
    this->mask = COMPONENT_MODEL | 
                 COMPONENT_POSITION | 
                 COMPONENT_ROTATION | 
                 COMPONENT_SCALE;

    this->phong = Program("phong.vert", "phong.frag");
    this->grid = Program("grid.vert", "grid.frag");
    this->bindGrid();
    this->projection = perspective((float)radians(45.0), 1280.0f / 720, 0.1f, 800.f);
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::render(shared_ptr<World> world)
{
    drawGrid(world);
    drawEntities(world);
}

void RenderingSystem::drawEntities(shared_ptr<World> world)
{
    glUseProgram(this->phong.prog);

    mat4 viewMat = world->camera.getViewMatrix();
    glUniformMatrix4fv(this->phong.getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(viewMat));
    glUniformMatrix4fv(this->phong.getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(this->projection));

    for (int i = 0; i < world->entities.size(); i++)
    {
        shared_ptr<Entity> entity = world->entities[i];
        if (entity->mask & this->mask != this->mask)
            continue;

        glUniformMatrix4fv(this->phong.getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(getModelMatrix(entity)));
        glUniform3f(this->phong.getUniformHandle("uLightPos"), 50, 5000, 50);

        shared_ptr<Model> model = dynamic_pointer_cast<Model>(entity->getComponent(COMPONENT_MODEL));
        for (int j = 0; j < model->meshes.size(); j++)
        {
            Mesh mesh = model->meshes.at(j);

            if (entity->mask & COMPONENT_PLAYER == COMPONENT_PLAYER) {
                shared_ptr<Player> player = dynamic_pointer_cast<Player>(entity->getComponent(COMPONENT_PLAYER));
                glUniform3fv(this->phong.getUniformHandle("uDiffuseColor"), 1, value_ptr(player->get_color()));
            }
            else {
                bindMaterial(model->materials[mesh.materialIdx]);
            }

            glBindVertexArray(mesh.VAO);
            glDrawArrays(GL_TRIANGLES, 0, mesh.indices.size());
            glBindVertexArray(0);
        }
    }
    glUseProgram(0);
}

mat4 RenderingSystem::getModelMatrix(shared_ptr<Entity> entity)
{
    mat4 modelMatrix = mat4();

    shared_ptr<Position> pos = dynamic_pointer_cast<Position>(entity->getComponent(COMPONENT_POSITION));
    shared_ptr<Rotation> rot = dynamic_pointer_cast<Rotation>(entity->getComponent(COMPONENT_ROTATION));
    shared_ptr<Scale> scale = dynamic_pointer_cast<Scale>(entity->getComponent(COMPONENT_SCALE));

    modelMatrix = glm::translate(modelMatrix, vec3(pos->position));

    modelMatrix = glm::scale(modelMatrix, vec3(scale->x, scale->y, scale->z));

    modelMatrix = glm::rotate(modelMatrix, rot->phi, vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, rot->theta, vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, rot->psi, vec3(0, 0, 1));  

    return modelMatrix;
}

void RenderingSystem::bindMaterial(Material material)
{
    //glUniform3fv(this->phong.getUniformHandle("uAmbientColor"), 1, value_ptr(material.ambient));
    glUniform3fv(this->phong.getUniformHandle("uDiffuseColor"), 1, value_ptr(material.diffuse));
    //glUniform3fv(this->phong.getUniformHandle("uSpecularColor"), 1, value_ptr(material.specular));
    //glUniform1fv(this->phong.getUniformHandle("uShininess"), 1, &material.shininess);
}

void RenderingSystem::bindGrid()
{
    int grid_max = 300;
    int step = 10;
    this->grid_points = vector<vec3>();
    for (int i = 0; i <= grid_max; i += step) {
        grid_points.push_back(vec3(i, 0, 0));
        grid_points.push_back(vec3(i, 0, grid_max));
        grid_points.push_back(vec3(0, 0, i));
        grid_points.push_back(vec3(grid_max, 0, i));
    }

    glGenVertexArrays(1, &GRID_VAO);
    glBindVertexArray(GRID_VAO);

    glGenBuffers(1, &GRID_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, GRID_VBO);
    glBufferData(GL_ARRAY_BUFFER, grid_points.size()*sizeof(vec3),
        &grid_points[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderingSystem::drawGrid(shared_ptr<World> world)
{
    glUseProgram(this->grid.prog);

    mat4 viewMat = world->camera.getViewMatrix();
    glUniformMatrix4fv(this->grid.getUniformHandle("uViewMatrix"), 1, GL_FALSE, value_ptr(viewMat));
    glUniformMatrix4fv(this->grid.getUniformHandle("uProjMatrix"), 1, GL_FALSE, value_ptr(this->projection));
    glUniformMatrix4fv(this->grid.getUniformHandle("uModelMatrix"), 1, GL_FALSE, value_ptr(mat4()));

    glBindVertexArray(GRID_VAO);
    glDrawArrays(GL_LINES, 0, this->grid_points.size());
    glBindVertexArray(0);

    glUseProgram(0);
}