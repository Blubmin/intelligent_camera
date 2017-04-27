#pragma once

#include <memory>

#include "ModelLoader.h"
#include "Model.h"

class World;

class EntityFactory
{
public:
    EntityFactory();
    ~EntityFactory();

    void createBoid(std::shared_ptr<World> world, float x, float y, float z);
    void createTree(std::shared_ptr<World> world, float x, float y, float z);

private:
    ModelLoader ldr;
};

