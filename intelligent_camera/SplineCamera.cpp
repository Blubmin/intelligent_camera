#include "SplineCamera.h"

#include <glm\gtc\matrix_transform.hpp>

using namespace glm;
using namespace std;

SplineCamera::SplineCamera() {};

SplineCamera::SplineCamera(shared_ptr<World> world)
{
    this->world = world;
    this->total_time = 0;
    this->speed = 5;
    this->step_size = .1;

    this->controlPoints = vector<vec3>();
    this->controlPoints.push_back(vec3(-20, 5, 0));
    this->controlPoints.push_back(vec3(0, 10, -20));
    this->controlPoints.push_back(vec3(20, 5, 0));
    this->controlPoints.push_back(vec3(0, 10, 20));

    for (int i = 0; i < this->controlPoints.size(); i++) {
        for (float step = 0.0; step < 1.0; step += this->step_size) {
            vec3 pos = calcPosition(i, step);

            float dist = 0;
            if (curveSteps.size() > 0) {
                dist = glm::distance(pos, curveSteps.back().pos);
                dist += curveSteps.back().dist;
            }

            CurveStep info;
            info.t = i + step;
            info.dist = dist;
            info.pos = pos;
            curveSteps.push_back(info);
        }
    }

    this->total_dist = curveSteps.back().dist + glm::distance(curveSteps.front().pos, curveSteps.back().pos);
    this->max_time = this->total_dist / this->speed;

    CurveStep info;
    info.t = controlPoints.size();
    info.dist = total_dist;
    info.pos = curveSteps.front().pos;
    curveSteps.push_back(info);
}

SplineCamera::~SplineCamera()
{
}

vec3 SplineCamera::calcPosition(int i, float step)
{
    vec3 p0 = controlPoints[(i - 1) % controlPoints.size()];
    vec3 p1 = controlPoints[i];
    vec3 p2 = controlPoints[(i + 1) % controlPoints.size()];
    vec3 p3 = controlPoints[(i + 2) % controlPoints.size()];

    vec3 pos = .5f * (
        (2.f * p1) +
        (-p0 + p2) * (step)+
        (2.f * p0 - 5.f * p1 + 4.f * p2 - p3) * (step * step) +
        (-p0 + 3.f * p1 - 3.f * p2 + p3) * (step * step * step)
        );

    return pos;
}

void SplineCamera::update(double timeElapsed)
{
    total_time += timeElapsed;
    total_time = fmod(total_time, max_time);
    float dist = total_time * speed;

    for (int i = 0; i < curveSteps.size(); i++) {
        if (curveSteps[i].dist < dist) continue;
        i--;
        CurveStep info = curveSteps[i];
        float remaining = dist - info.dist;
        float step = remaining / (curveSteps[(i + 1) % curveSteps.size()].dist - info.dist) * this->step_size;
        this->pos = calcPosition((int)info.t, fmod(info.t, 1) + step);
        return;
    }
}

mat4 SplineCamera::getViewMatrix()
{
    return lookAt(this->pos, this->lookAtPt, vec3(0, 1, 0));
}
