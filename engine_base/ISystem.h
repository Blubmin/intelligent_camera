#pragma once

class Scene;

class ISystem {
public:
    ISystem() {};
    ~ISystem() {};
    virtual void update(float time_elapsed, Scene* scene) = 0;
    virtual void draw() = 0;
};

