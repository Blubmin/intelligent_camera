#include "TextureRenderer.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <engine_base\GLSL.h>

TextureRenderer::TextureRenderer(IRenderer * renderer, int width, int height) : IRenderer(0) {
    _child = renderer;

    _width = width;
    _height = height;

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &_depth);
    glBindTexture(GL_TEXTURE_2D, _depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_STENCIL, _width, _height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depth, 0);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextureRenderer::~TextureRenderer() {}

void TextureRenderer::draw(Scene * scene, ICamera * cam) {
    GLint prev_fbo;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prev_fbo);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, _width, _height);

    bool stencil = glIsEnabled(GL_STENCIL_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  

    _child->draw(scene, cam);

    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    glBindFramebuffer(GL_FRAMEBUFFER, prev_fbo);
    stencil ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}

GLuint TextureRenderer::fbo() {
    return _fbo;
}

GLuint TextureRenderer::color() {
    return _tex;
}

GLuint TextureRenderer::depth() {
    return _depth;
}
