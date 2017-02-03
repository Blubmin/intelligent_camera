#include "EntityFactory.h"

#include "Position.h"
#include "World.h"

using namespace std;

EntityFactory::EntityFactory()
{
    this->ldr = ModelLoader();
}


EntityFactory::~EntityFactory()
{
}

void EntityFactory::createTree(std::shared_ptr<World> world)
{
    shared_ptr<Entity> tree = make_shared<Entity>(Entity());

    shared_ptr<Model> model;
    try
    {
        model = ldr.getModelByName("tree");
    }
    catch (const exception& e)
    {
        model = ldr.loadModelByName("Models/lowpolytree.obj", "tree");
    }
    tree->addComponent(model);

    tree->addComponent(make_shared<Position>(Position()));

    world->entities.push_back(tree);
}
