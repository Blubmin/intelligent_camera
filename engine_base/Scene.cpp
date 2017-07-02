#include "Scene.h"

#include <fstream>
#include <iostream>
#include <set>

#include <glm\glm.hpp>

#include <rapidjson\document.h>
#include <rapidjson\filereadstream.h>
#include <rapidjson\filewritestream.h>
#include <rapidjson\prettywriter.h>

#include "Component.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "FlatRenderer.h"
#include "ICamera.h"
#include "IRenderer.h"
#include "ISystem.h"
#include "ModelLoader.h"
#include "Position.h"
#include "Rotation.h"
#include "Scale.h"

using namespace glm;
using namespace rapidjson;
using namespace std;

char const * Scene::FilterPatterns[1] = {"*.scene"};

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
    FILE* fp;
    fopen_s(&fp, filename.c_str(), "w");

    char writeBuffer[JSON_BUFFER_SIZE];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    PrettyWriter<FileWriteStream> writer(os);

    Document doc(kObjectType);
    Document::AllocatorType& alloc = doc.GetAllocator();

    // models
    Value models(kArrayType);
    for (auto k : keys()) {
        Value model(kObjectType);
        Value key(k.c_str(), alloc);
        model.AddMember("key", key, alloc);
        Value file(ModelLoader::filename(k).c_str(), alloc);
        model.AddMember("file", file, alloc);
        models.PushBack(model, alloc);
    }
    doc.AddMember("models", models, alloc);
    // end models

    // entities
    Value entities(kArrayType);
    for (auto e : _entites) {
        Value entity(kObjectType);
        Value tag(e->tag().c_str(), alloc);
        entity.AddMember("tag", tag, alloc);

        if (e->check_mask(COMPONENT_POSITION)) {
            Position* c = dynamic_cast<Position*>(e->getComponent(COMPONENT_POSITION));
            Value pos(kObjectType);
            pos.AddMember("x", c->position.x, alloc);
            pos.AddMember("y", c->position.y, alloc);
            pos.AddMember("z", c->position.z, alloc);
            entity.AddMember("pos", pos, alloc);
        }

        if (e->check_mask(COMPONENT_ROTATION)) {
            Rotation* c = dynamic_cast<Rotation*>(e->getComponent(COMPONENT_ROTATION));
            Value rot(kObjectType);
            rot.AddMember("phi", c->phi, alloc);
            rot.AddMember("theta", c->theta, alloc);
            rot.AddMember("psi", c->psi, alloc);
            entity.AddMember("rot", rot, alloc);
        }

        if (e->check_mask(COMPONENT_SCALE)) {
            Scale* c = dynamic_cast<Scale*>(e->getComponent(COMPONENT_SCALE));
            Value scale(kObjectType);
            scale.AddMember("x", c->x, alloc);
            scale.AddMember("y", c->y, alloc);
            scale.AddMember("z", c->z, alloc);
            entity.AddMember("scale", scale, alloc);
        }

        entities.PushBack(entity, alloc);
    }
    doc.AddMember("entities", entities, alloc);
    // end entites
    doc.Accept(writer);
    fclose(fp);
}

void Scene::import(const std::string & filename) {
    FILE* fp;
    fopen_s(&fp, filename.c_str(), "r");

    char readBuffer[JSON_BUFFER_SIZE];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document doc;
    doc.ParseStream(is);
    
    assert(doc.IsObject());
    auto d = doc.GetObject();
    for (auto it = d.MemberBegin(); it != d.MemberEnd(); it++) {
        cout << it->name.GetString() << endl;
    }
    for (const auto& m : d["models"].GetArray()) {
        ModelLoader::loadModelByName(m["file"].GetString(), m["key"].GetString());
    }

    for (const auto& e : d["entities"].GetArray()) {
        string tag = e["tag"].GetString();
        const Value& pos = e["pos"];
        const Value& rot = e["rot"];
        const Value& scl = e["scale"];

        vec3 position = vec3(pos["x"].GetFloat(), pos["y"].GetFloat(), pos["z"].GetFloat());
        vec3 rotation = vec3(rot["phi"].GetFloat(), rot["theta"].GetFloat(), rot["psi"].GetFloat());
        vec3 scale = vec3(scl["x"].GetFloat(), scl["y"].GetFloat(), scl["z"].GetFloat());

        Entity* entity = EntityFactory::create_entity(tag, position, rotation, scale);
        add_entity(entity);
    }

    fclose(fp);
}

std::set<std::string> Scene::keys() {
    set<string> keys;
    for (auto e : _entites) {
        keys.insert(e->tag());
    }
    return keys;
}