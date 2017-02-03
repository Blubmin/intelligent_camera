#pragma once

#include <vector>

#include <assimp\scene.h>

#include "Component.h"
#include "Mesh.h"

class Model : public Component
{
public:
    Model();
    Model(const aiScene* scene);
    ~Model();

    std::vector<Mesh> meshes;
};

