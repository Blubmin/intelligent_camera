#pragma once

#include <map>
#include <memory>
#include <string>

#include <glm\glm.hpp>

#include "Component.h"

class Entity
{
    unsigned long _mask;
    std::string _tag;

public:
    Entity();
    Entity(std::string tag);
    ~Entity();

    void addComponent(Component* component);
    Component* getComponent(unsigned long mask);
    bool check_mask(unsigned long mask);
    std::string tag();

    std::map<unsigned long, Component*> components;
};

