#pragma once

#define JSON_BUFFER_SIZE 65536

#include <fstream>
#include <memory>
#include <string>

#include <rapidjson\document.h>

class World;

class PlayerMovementSystem
{
    unsigned long mask;
    std::shared_ptr<rapidjson::Document> doc;
    char buffer[JSON_BUFFER_SIZE];
    unsigned long frame;

public:
    PlayerMovementSystem();
    PlayerMovementSystem(std::string filename);
    ~PlayerMovementSystem();
    void move(std::shared_ptr<World> world);
};

