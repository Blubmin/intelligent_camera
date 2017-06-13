#pragma once

class Scene;
class ICamera;

class IRenderer {

protected:
    unsigned long _mask;

public:
    IRenderer(unsigned long mask) {
        _mask = mask;
    };
    ~IRenderer() {};
    virtual void draw(Scene* scene, ICamera* cam) = 0;
};

