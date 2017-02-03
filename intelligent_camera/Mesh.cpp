#include "Mesh.h"

using namespace glm;
using namespace std;

Mesh::Mesh()
{
}

Mesh::Mesh(const aiMesh* mesh)
{
    this->data = vector<vec3>();

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexData vert = VertexData();
        aiVector3D pos = mesh->mVertices[i];
        vert.pos = vec3(pos.x, pos.y, pos.z);
        this->data.push_back(vert.pos);
    }

    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    bindBuffers();
}

Mesh::~Mesh()
{
}

void Mesh::bindBuffers()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(vec3),
        &data[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &IND);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}