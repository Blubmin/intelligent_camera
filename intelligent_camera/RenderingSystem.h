#pragma once

#include <memory>

#include <glm\glm.hpp>

#include "Program.h"

class World;

class RenderingSystem
{
public:
    RenderingSystem();
    ~RenderingSystem();

    void render(std::shared_ptr<World> world);

private:
    unsigned long mask;
    Program phong;
    glm::mat4 projection;
};

