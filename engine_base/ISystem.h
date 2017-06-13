#pragma once

class Entity;

class ISystem {
public:
    ISystem() {};
    ~ISystem() {};
    virtual void update(float time_elapsed, Entity* entity) = 0;
    virtual void draw() = 0;
};

