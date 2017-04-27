#include "Player.h"

using namespace glm;

Player::Player(int label) : Component(COMPONENT_PLAYER) {
    this->label = label;
}

Player::~Player() {}

vec3 Player::get_color() {
    vec3 colors[] = {
        vec3(1, 0, 0),
        vec3(0, 1, 0),
        vec3(0, 0, 1),
        vec3(1, 1, 0),
        vec3(0, 1, 1),
        vec3(1, 0, 1)
    };
    return colors[label];
}