#pragma once

#include <map>
#include <memory>
#include <string>

#include "Component.h"

class Entity
{
public:
    Entity();
    ~Entity();

    void addComponent(std::shared_ptr<Component> component);
    std::shared_ptr<Component> getComponent(unsigned long mask);

    unsigned long mask;
    std::map<unsigned long, std::shared_ptr<Component>> components;
};

