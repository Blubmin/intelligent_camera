#pragma once

#include <engine_base\Component.h>

extern unsigned long COMPONENT_SELECTED;

class Selected : public Component {

    bool _selected;

public:
    Selected();
    ~Selected();

    void select();
    void deselect();
    const bool& selected();
};

