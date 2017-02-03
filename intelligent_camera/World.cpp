#include "World.h"

using namespace std;

World::World()
{
    this->camera = Camera();
    this->renderer = RenderingSystem();
    this->factory = EntityFactory();
}

World::~World()
{
}

void World::init()
{
    factory.createTree(this->shared_from_this());
}

void World::step(double elapsedTime)
{
    renderer.render(this->shared_from_this());
}
