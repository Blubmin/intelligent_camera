#include "Scene.h"

#include <fstream>

#include "FlatRenderer.h"
#include "ICamera.h"
#include "IRenderer.h"
#include "ISystem.h"

using namespace glm;
using namespace std;

Scene::Scene() {
    _dir_light = vec3(1, -1, 1);
    _renderer = new FlatRenderer();
}

Scene::~Scene() {
    delete _renderer;
}

void Scene::set_renderer(IRenderer * renderer) {
    delete _renderer;
    _renderer = renderer;
}

void Scene::add_system(ISystem * system) {
    _systems.push_back(system);
}

void Scene::add_entity(Entity * entity) {
    _entites.push_back(entity);
}

std::vector<Entity*>& Scene::entities() {
    return _entites;
}

glm::vec3 Scene::dir_light() {
    return _dir_light;
}

void Scene::update(float time_elapsed) {
    for (auto s : _systems) {
         s->update(time_elapsed, this);    
    }
}

void Scene::draw(ICamera* cam) {
    _renderer->draw(this, cam);
}

void Scene::save(const std::string & filename)
{
    ofstream out;
    out.open(filename, ofstream::trunc);

    out << "{" << endl;
    out << "\tmodels: [" << endl;

    out << "]" << endl;
    out << "}" << endl;
}
