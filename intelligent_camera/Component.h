#pragma once

class Component
{
public:
    Component();
    Component(unsigned long mask);
    virtual ~Component();

    unsigned long mask;
};

enum ComponentType : unsigned long
{
    COMPONENT_MODEL = 1 << 0,
    COMPONENT_POSITION = 1 << 1
};

