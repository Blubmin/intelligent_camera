#pragma once

#include <string>
#include <vector>

#include <assimp\scene.h>

#include "Component.h"
#include "Material.h"
#include "Mesh.h"

class Model : public Component
{
public:
    Model();
    Model(const aiScene* scene);
    ~Model();

    void calc_hulls();

    std::vector<Mesh> meshes;
    std::vector<Mesh> hulls;
    std::vector<Material> materials;

private:
    void shiftUpToGround();
    void shrink();
};

