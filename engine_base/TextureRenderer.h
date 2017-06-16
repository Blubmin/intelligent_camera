#pragma once
#include "IRenderer.h"

#include <GL\glew.h>

class ICamera;

class TextureRenderer :
    public IRenderer {

    IRenderer* _child;
    GLuint _fbo;
    GLuint _tex;
    GLuint _depth;

    int _width;
    int _height;

public:
    TextureRenderer(IRenderer* renderer, int width, int height);
    ~TextureRenderer();

    void draw(Scene* scene, ICamera* cam);
    GLuint fbo();
    GLuint color();
    GLuint depth();
};

