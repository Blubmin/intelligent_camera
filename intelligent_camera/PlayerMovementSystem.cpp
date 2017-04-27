#include "PlayerMovementSystem.h"

#include <iostream>
#include <cstdio>

#include <rapidjson\filereadstream.h>

#include "Position.h"
#include "World.h"

using namespace rapidjson;
using namespace std;

PlayerMovementSystem::PlayerMovementSystem() {}

PlayerMovementSystem::PlayerMovementSystem(string filename) {
    this->mask = COMPONENT_POSITION | COMPONENT_PLAYER;
    FILE* fp;
    fopen_s(&fp, filename.c_str(), "r");
    FileReadStream is = FileReadStream(fp, this->buffer, JSON_BUFFER_SIZE);
    doc = make_shared<Document>();
    doc->ParseStream(is);
    frame = 1;
}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::move(shared_ptr<World> world) {
    int player = 0;
    for (int i = 0; i < world->entities.size(); i++) {
        shared_ptr<Entity> entity = world->entities[i];
        if (entity->mask & this->mask != this->mask)
            continue;

        shared_ptr<Position> pos = dynamic_pointer_cast<Position>(entity->getComponent(COMPONENT_POSITION));
        pos->position.x = (*doc)[frame]["players"][player]["position"][0].GetDouble() / 10.0f;
        pos->position.y = (*doc)[frame]["players"][player]["position"][1].GetDouble() / 10.0f;
        pos->position.z = (*doc)[frame]["players"][player]["position"][2].GetDouble() / 10.0f;
        player += 1;
    }
    frame += 1;
    frame %= (*doc).Capacity();
}