#pragma once

#include <memory>
#include <vector>

#include <glm\glm.hpp>

#include "Entity.h"
#include "Material.h"
#include "Program.h"

class Mesh;
class World;

class RenderingSystem
{
public:
    RenderingSystem();
    ~RenderingSystem();

    void render(std::shared_ptr<World> world);   

private:
    glm::mat4 getModelMatrix(std::shared_ptr<Entity> entity);
    void bindMaterial(Material material);

    void bindGrid();
    void drawGrid(std::shared_ptr<World> world);
    void drawMesh(const Mesh & mesh, GLint mode);
    void drawEntities(std::shared_ptr<World> world);

    unsigned long mask;
    Program phong;
    Program grid;
    Program hull;
    std::vector<glm::vec3> grid_points;
    std::vector<GLuint> grid_idx;
    GLuint GRID_VBO;
    GLuint GRID_VAO;
    GLuint GRID_IDX;
    glm::mat4 projection;
};

