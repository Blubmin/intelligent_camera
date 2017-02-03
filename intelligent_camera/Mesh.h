#pragma once

#include <vector>

#include <assimp\scene.h>
#include <GL\glew.h>
#include <glm\glm.hpp>

struct VertexData {
    glm::vec3 pos;
};

class Mesh
{
public:
    Mesh();
    Mesh(const aiMesh* scene);
    ~Mesh();

    std::vector<glm::vec3> data;
    std::vector<GLuint> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint IND;

private:
    void bindBuffers();
};

