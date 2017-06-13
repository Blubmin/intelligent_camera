#pragma once

#include <memory>
#include <string>

#include <glm\glm.hpp>

#include "Entity.h"
#include "ModelLoader.h"

class World;

class EntityFactory
{
public:
    static Entity* create_entity(std::string model_id, glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3(1));
};

