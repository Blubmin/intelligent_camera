#pragma once

#include <memory>
#include <vector>

#include "Camera.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "RenderingSystem.h"

class World : public std::enable_shared_from_this<World>
{
public:
    World();
    ~World();

    void init();
    void step(double elapsedTime);

    std::vector<std::shared_ptr<Entity>> entities;
    EntityFactory factory;
    Camera camera;
    RenderingSystem renderer;
};

