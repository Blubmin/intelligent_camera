#pragma once

#include <memory>
#include <vector>

#include "Camera.h"

class World;

class SplineCamera : public Camera
{
public:
    SplineCamera();
    SplineCamera(std::shared_ptr<World>);
    ~SplineCamera();

    void update(double timeElapsed);
    glm::mat4 getViewMatrix();

private:

    struct CurveStep {
        float t;
        float dist;
        glm::vec3 pos;
    };

    glm::vec3 calcPosition(int i, float step);

    std::weak_ptr<World> world;
    std::vector<glm::vec3> controlPoints;
    std::vector<CurveStep> curveSteps;
    glm::vec3 lookAtPt;

    double total_dist;
    double total_time;
    double max_time;
    double speed;
    double step_size;
};

