#include "QuickHull.h"

#include <climits>
#include <vector>

#include <glm\glm.hpp>

#include "Mesh.h"

using namespace glm;
using namespace std;

QuickHull::QuickHull() {}

QuickHull::~QuickHull() {}

Mesh QuickHull::GenerateHull(const Mesh & mesh) {

    if (mesh.verts.size() <= 4) return mesh;

    vector<vec3> verts(mesh.verts.size());
    copy(mesh.verts.begin(), mesh.verts.end(), verts.begin());

    vector<vec3> min_max = vector<vec3>(6);
    min_max[1] = vec3(FLT_MAX, 0, 0);
    min_max[2] = vec3(0, FLT_MAX, 0);
    min_max[3] = vec3(0, 0, FLT_MAX);
    min_max[4] = vec3(FLT_MIN, 0, 0);
    min_max[5] = vec3(0, FLT_MIN, 0);
    min_max[6] = vec3(0, 0, FLT_MIN);

    // Finds x, y, z extremes
    for (auto v = verts.begin(); v != verts.end(); v++) {
        // Finds minimum extremes
        min_max[0] = min_max[0].x < v->x ? min_max[0] : *v;
        min_max[1] = min_max[1].y < v->y ? min_max[1] : *v;
        min_max[2] = min_max[2].z < v->z ? min_max[2] : *v;

        // Find maximum extremes
        min_max[3] = min_max[3].x > v->x ? min_max[3] : *v;
        min_max[4] = min_max[4].y > v->y ? min_max[4] : *v;
        min_max[5] = min_max[5].z > v->z ? min_max[5] : *v;
    }

    vector<float> lengths = vector<float>(15);

    int p1, p2;
    float max_dist = -1;

    // Finds furthest apart extremes
    for (int i = 0; i < min_max.size(); i++) {
        for (int j = i + 1; j < 6; j++) {
            float dist = distance(min_max[i], min_max[j]);
            if (max_dist < dist) {
                max_dist = dist;
                p1 = i;
                p2 = j;
            }
        }
    }

    vec3 v1 = min_max[p1];
    vec3 v2 = min_max[p2];
    vec3 v3;
    max_dist = -1;

    // Finds extreme that creates the largest triangle with the previous two
    for (int i = 0; i < min_max.size(); i++) {
        vec3 v = min_max[i];
        float dist = length(cross(v - v1, v - v2)) / length(v2 - v1);
        if (max_dist < dist) {
            max_dist = dist;
            v3 = v;
        }
    }

    vec3 norm = normalize(cross(v2 - v1, v3 - v1));

    vec3 v4;
    max_dist = -1;

    // Finds fourth point that create largest overall tetrahedron
    for (auto v = verts.begin(); v != verts.end(); v++) {
        float dist = dot(norm, *v - v1);
        if (max_dist < dist) {
            max_dist = dist;
            v4 = *v;
        }
    }

    return Mesh();
}


class Triangle {

};