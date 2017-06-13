#pragma once

#include <memory>

#include "engine_base\Model.h"
#include "engine_base\ModelLoader.h"

class World;

class EntityFactory
{
public:
    EntityFactory();
    ~EntityFactory();

    void createBoid(std::shared_ptr<World> world, float x, float y, float z);
    void createTree(std::shared_ptr<World> world, float x, float y, float z);
    void createPlayer(std::shared_ptr<World> world, float x, float y, float z, int label);
};

