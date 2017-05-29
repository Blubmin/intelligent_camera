#pragma once

#include <engine_base\Component.h>

#include <glm\glm.hpp>

class Player : public Component {
    int label;
public:
    Player(int label);
    ~Player();
    glm::vec3 get_color();
};

