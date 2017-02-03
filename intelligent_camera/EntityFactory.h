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

    void createTree(std::shared_ptr<World> world);

private:
    ModelLoader ldr;
};

