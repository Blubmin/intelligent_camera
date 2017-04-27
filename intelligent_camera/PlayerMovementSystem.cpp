#include "PlayerMovementSystem.h"

#include <fstream>
#include <iostream>
#include <cstdio>

#include <rapidjson\filereadstream.h>

#include "Globals.h"
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
    //ifstream file;
    //file.open(filename);
    //string json = "";
    //string line = "";
    
    //while (getline(file, line)) {
    //    json += line;
    //}
    doc = make_shared<Document>();
    doc->ParseStream(is);
    //doc->Parse(json.c_str());
    cur_frame = 1;
    total_frames = (*doc).Capacity();
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
        pos->position.x = (*doc)[frame]["players"][player]["position"][0].GetDouble() / 5.0f;
        pos->position.y = (*doc)[frame]["players"][player]["position"][1].GetDouble() / 5.0f;
        pos->position.z = (*doc)[frame]["players"][player]["position"][2].GetDouble() / 5.0f;
        player += 1;
    }
    frame += 1;
    frame %= (*doc).Capacity();
    if (!frame) frame = 1;
    cur_frame = frame;
}