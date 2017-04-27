#pragma once

#include "Component.h"

#include <glm\glm.hpp>

class Player : public Component {
    int label;
public:
    Player(int label);
    ~Player();
    glm::vec3 get_color();
};

