#include "Model.h"

Model::Model() : Component(COMPONENT_MODEL)
{
}

Model::Model(const aiScene* scene) : Component(COMPONENT_MODEL)
{
    for (int i = 0; i < scene->mNumMeshes; i++)
    {
        Mesh mesh = Mesh(scene->mMeshes[i]);
        meshes.push_back(mesh);
    }
}

Model::~Model()
{
}
