#include "EntityFactory.h"

#include "Player.h"
#include "Position.h"
#include "Rotation.h"
#include "Scale.h"
#include "World.h"

using namespace std;

EntityFactory::EntityFactory()
{
    this->ldr = ModelLoader();
}


EntityFactory::~EntityFactory()
{
}

void EntityFactory::createBoid(std::shared_ptr<World> world, float x, float y, float z)
{
    shared_ptr<Entity> boid = make_shared<Entity>(Entity());

    shared_ptr<Model> model;
    try
    {
        model = ldr.getModelByName("bird");
    }
    catch (const exception& e)
    {
        model = ldr.loadModelByName("Models/bird.3ds", "bird");
    }
    boid->addComponent(model);

    boid->addComponent(make_shared<Position>(Position(x, y, z)));
    boid->addComponent(make_shared<Rotation>(-90, 0, 0));
    boid->addComponent(make_shared<Scale>());

    world->entities.push_back(boid);
}

void EntityFactory::createTree(std::shared_ptr<World> world, float x, float y, float z)
{
    shared_ptr<Entity> tree = make_shared<Entity>(Entity());

    shared_ptr<Model> model;
    try
    {
        model = ldr.getModelByName("tree");
    }
    catch (const exception& e)
    {
        model = ldr.loadModelByName("Models/tree.fbx", "tree");
    }
    tree->addComponent(model);
    tree->addComponent(make_shared<Position>(Position(x, y, z)));
    tree->addComponent(make_shared<Rotation>());
    tree->addComponent(make_shared<Scale>(5));

    world->entities.push_back(tree);
}


void EntityFactory::createPlayer(std::shared_ptr<World> world, float x, float y, float z, int label)
{
    shared_ptr<Entity> player = make_shared<Entity>(Entity());

    shared_ptr<Model> model;
    try
    {
        model = ldr.getModelByName("tree");
    }
    catch (const exception& e)
    {
        model = ldr.loadModelByName("Models/sphere.obj", "tree");
    }
    player->addComponent(model);
    player->addComponent(make_shared<Player>(Player(label)));
    player->addComponent(make_shared<Position>(Position(x, y, z)));
    player->addComponent(make_shared<Rotation>());
    player->addComponent(make_shared<Scale>(5));

    world->entities.push_back(player);
}