#include "DebugRenderer.h"

#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Program.h"

using namespace glm;
using namespace std;

DebugRenderer::DebugRenderer() : DebugRenderer(nullptr, vec2(), 1.f) {}

DebugRenderer::DebugRenderer(IRenderer* renderer, glm::vec2 pos, float scale) : IRenderer(0) {
    _prog = new Program("debug.vert", "debug.frag");
    _child = renderer;

    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    _pos = vec2((pos.x / (float)width) * 2, -(pos.y / (float)height) * 2);
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

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLuint depth;
    glGenTextures(1, &depth);
    glBindTexture(GL_TEXTURE_2D, depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DebugRenderer::~DebugRenderer() {}

void DebugRenderer::draw(Scene * scene, ICamera * camera) {
    GLint prev_fbo;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prev_fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _child->draw(scene, camera);
    glBindFramebuffer(GL_FRAMEBUFFER, prev_fbo);

    glUseProgram(_prog->prog);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _tex);
    
    glUniform1i(_prog->uniform("uTex"), 0);
    glUniform2fv(_prog->uniform("uPos"), 1, value_ptr(_pos));
    glUniform1f(_prog->uniform("uScale"), _scale);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);
}