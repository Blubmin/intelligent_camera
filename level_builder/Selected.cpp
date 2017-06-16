#include "Selected.h"

unsigned long COMPONENT_SELECTED = 1 << 5;

Selected::Selected() : Component(COMPONENT_SELECTED) {
    _selected = false;
}


Selected::~Selected() {}


void Selected::select() {
    _selected = true;
}


void Selected::deselect() {
    _selected = false;
}


const bool & Selected::selected() {
    return _selected;
}
