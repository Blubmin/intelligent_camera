#include "World.h"

#include "SplineCamera.h"

using namespace std;

World::World()
{
    this->renderer = RenderingSystem();
    this->factory = EntityFactory();
}

World::~World()
{
}

void World::init()
{
    this->camera = SplineCamera(this->shared_from_this());
    factory.createBoid(this->shared_from_this(), 0, 6, 0);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            factory.createTree(this->shared_from_this(), i * 5, 0, j * 5);
}

void World::step(double elapsedTime)
{
    camera.update(elapsedTime);
    renderer.render(this->shared_from_this());
}
