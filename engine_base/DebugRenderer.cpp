#include "DebugRenderer.h"

#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Program.h"

using namespace glm;
using namespace std;

DebugRenderer::DebugRenderer(GLuint texture, glm::vec2 pos, float scale) : IRenderer(0) {
    _prog = new Program("debug.vert", "debug.frag");
    _tex = texture;

    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    _pos = pos;
    _scale = scale;

    vector<vec3> verts;
    verts.push_back(vec3(1, 1, 0));
    verts.push_back(vec3(1, -1, 0));
    verts.push_back(vec3(-1, -1, 0));
    verts.push_back(vec3(-1, 1, 0));

    vector<vec2> tex;
    tex.push_back(vec2(1, 1));
    tex.push_back(vec2(1, 0));
    tex.push_back(vec2(0, 0));
    tex.push_back(vec2(0, 1));

    vector<GLuint> indices;
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    GLuint vbo[2];
    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(vec3),
        verts.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tex.size()*sizeof(vec2),
        tex.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint ind;
    glGenBuffers(1, &ind);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    
}

DebugRenderer::~DebugRenderer() {}

void DebugRenderer::draw(Scene * scene, ICamera * camera) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

    glUseProgram(_prog->prog);
    glViewport(_pos.x, _pos.y, width * _scale, height * _scale);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _tex);
    
    glUniform1i(_prog->uniform("uTex"), 0);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}

GLuint DebugRenderer::texture() {
    return _tex;
}
