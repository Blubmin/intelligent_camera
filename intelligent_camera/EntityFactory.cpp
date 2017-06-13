#include "EntityFactory.h"

#include <engine_base\Position.h>
#include <engine_base\Rotation.h>
#include <engine_base\Scale.h>

#include "Player.h"
#include "World.h"

using namespace std;

EntityFactory::EntityFactory()
{
    ModelLoader::init("Models");
}


EntityFactory::~EntityFactory()
{
}

void EntityFactory::createBoid(std::shared_ptr<World> world, float x, float y, float z)
{
    shared_ptr<Entity> boid = make_shared<Entity>(Entity("bird"));

    Model* model;
    try
    {
        model = ModelLoader::getModelByName("bird");
    }
    catch (const exception& e)
    {
        model = ModelLoader::loadModelByName("Models/bird.3ds", "bird");
    }
    boid->addComponent(model);

    boid->addComponent(new Position(Position(x, y, z)));
    boid->addComponent(new Rotation(-90, 0, 0));
    boid->addComponent(new Scale());

    world->entities.push_back(boid);
}

void EntityFactory::createTree(std::shared_ptr<World> world, float x, float y, float z)
{
    shared_ptr<Entity> tree = make_shared<Entity>(Entity("tree"));

    Model* model;
    try
    {
        model = ModelLoader::getModelByName("tree2");
    }
    catch (const exception& e)
    {
        model = ModelLoader::loadModelByName("Models/tree2.obj", "tree");
    }
    
    tree->addComponent(model);

    tree->addComponent(new Position(x, y, z));
    tree->addComponent(new Rotation());
    tree->addComponent(new Scale(5));

    world->entities.push_back(tree);
}


void EntityFactory::createPlayer(std::shared_ptr<World> world, float x, float y, float z, int label)
{
    shared_ptr<Entity> player = make_shared<Entity>(Entity("player"));

    Model* model;
    try
    {
        model = ModelLoader::getModelByName("tree");
    }
    catch (const exception& e)
    {
        model = ModelLoader::loadModelByName("Models/sphere.obj", "tree");
    }
    player->addComponent(model);
    player->addComponent(new Player(Player(label)));
    player->addComponent(new Position(Position(x, y, z)));
    player->addComponent(new Rotation());
    player->addComponent(new Scale(5));

    world->entities.push_back(player);
}