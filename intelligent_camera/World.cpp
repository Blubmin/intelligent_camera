#include "World.h"

#include <glm\glm.hpp>

#include "DebugCamera.h"

using namespace glm;
using namespace std;

World::World() {
    this->renderer = RenderingSystem();
    this->factory = EntityFactory();
    this->renderer = RenderingSystem();
    this->movement = PlayerMovementSystem("positions.json");
}

World::~World() {
    delete camera;
}

void World::init() {
    // this->camera = SplineCamera(this->shared_from_this());
    // this->camera = DebugCamera(vec3(-10, 5, -10));
    // this->camera = new DebugCamera(vec3(10, 5, 10));
    this->camera = new BlenderCamera();
    //factory.createBoid(this->shared_from_this(), 0, 0, 0);
    factory.createTree(this->shared_from_this(), 0, 0, 0);
    //factory.createPlayer(this->shared_from_this(), 0, 0, 0, 0);
    //factory.createPlayer(this->shared_from_this(), 0, 0, 0, 1);
    //factory.createPlayer(this->shared_from_this(), 0, 0, 0, 2);
    //factory.createPlayer(this->shared_from_this(), 0, 0, 0, 3);
    //factory.createPlayer(this->shared_from_this(), 0, 0, 0, 4);
    //factory.createPlayer(this->shared_from_this(), 0, 0, 0, 5);
}

void World::step(double elapsedTime) {
    camera->update(elapsedTime);
    movement.move(this->shared_from_this());
    renderer.render(this->shared_from_this());
}
