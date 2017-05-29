#pragma once

#include <memory>
#include <vector>

#include <engine_base\Entity.h>

#include "Camera.h"
#include "BlenderCamera.h"
#include "EntityFactory.h"
#include "RenderingSystem.h"
#include "PlayerMovementSystem.h"

class World : public std::enable_shared_from_this<World>
{
public:
    World();
    ~World();

    void init();
    void step(double elapsedTime);

    std::vector<std::shared_ptr<Entity>> entities;
    EntityFactory factory;
    // SplineCamera camera;
    // DebugCamera camera;
    // BlenderCamera camera;
    Camera* camera;
    RenderingSystem renderer;
    PlayerMovementSystem movement;
};

