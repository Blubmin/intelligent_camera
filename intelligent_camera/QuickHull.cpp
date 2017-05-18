#include "QuickHull.h"

#include <algorithm>
#include <climits>
#include <list>
#include <stack>
#include <vector>

#include <glm\glm.hpp>

#include "Mesh.h"

using namespace glm;
using namespace std;

struct HalfSpaceCheck {
    vec3 _norm, _v1;
    HalfSpaceCheck(vec3 norm, vec3 v1) {
        _norm = norm;
        _v1 = v1;
    }

    bool operator() (vec3 v) {
        return dot(_norm, v - _v1) > 0;
    }
};

class Triangle {

    vec3 norm;

    bool is_out(vec3 v) {
        return dot(norm, v - v1) < 0;
    }

public:
    vec3 v1, v2, v3;
    vector<vec3> verts;

    Triangle() {};

    Triangle(vec3 v1, vec3 v2, vec3 v3, vector<vec3> & verts) {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
        this->norm = normalize(cross(v2 - v1, v3 - v1));

        if (std::find(verts.begin(), verts.end(), v1) != verts.end())
            verts.erase(remove(verts.begin(), verts.end(), v1));
        if (std::find(verts.begin(), verts.end(), v2) != verts.end())
            verts.erase(remove(verts.begin(), verts.end(), v2));
        if (std::find(verts.begin(), verts.end(), v3) != verts.end())
            verts.erase(remove(verts.begin(), verts.end(), v3));

        vector<vec3>::iterator iter = remove_if(verts.begin(), verts.end(), HalfSpaceCheck(norm, v1));
        this->verts = vector<vec3>(iter, verts.end());
        verts.erase(iter, verts.end());
    }

    ~Triangle() {};

    bool tri_dot(vec3 v1, vec3 v2, vec3 v3) {
        vec3 norm = normalize(cross(v2 - v1, v3 - v1));
        return dot(this->norm, norm) > 0;
    }

    bool is_edge() {
        return verts.empty();
    }

    vec3 max_point() {
        vec3 p;
        float max_dist = -1;
        for (auto v = verts.begin(); v != verts.end(); v++) {
            float dist = dot(norm, *v - v1);
            if (max_dist < dist) {
                max_dist = dist;
                p = *v;
            }
        }
        return p;
    }

    static Triangle create_triangle(Triangle source, vec3 v1, vec3 v2, vec3 v3, vector<vec3>& verts, bool same_dir = true) {
        Triangle t1;
        if (source.tri_dot(v1, v2, v3) == same_dir) {
            t1 = Triangle(v1, v2, v3, verts);
        }
        else {
            t1 = Triangle(v1, v3, v2, verts);
        }
        return t1;
    }
};

QuickHull::QuickHull() {}

QuickHull::~QuickHull() {}

Mesh QuickHull::GenerateHull(const Mesh & mesh) {

    if (mesh.verts.size() <= 4) return mesh;

    vector<vec3> verts(mesh.verts.size());
    copy(mesh.verts.begin(), mesh.verts.end(), verts.begin());

    vector<vec3> min_max = vector<vec3>(6);
    min_max[0] = vec3(FLT_MAX, 0, 0);
    min_max[1] = vec3(0, FLT_MAX, 0);
    min_max[2] = vec3(0, 0, FLT_MAX);
    min_max[3] = vec3(FLT_MIN, 0, 0);
    min_max[4] = vec3(0, FLT_MIN, 0);
    min_max[5] = vec3(0, 0, FLT_MIN);

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
        for (int j = i + 1; j < min_max.size(); j++) {
            float dist = abs(distance(min_max[i], min_max[j]));
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
        float dist = abs(length(cross(v - v1, v - v2)) / length(v2 - v1));
        if (max_dist < dist) {
            max_dist = dist;
            v3 = v;
        }
    }

    vec3 norm = normalize(cross(v2 - v1, v3 - v1));

    vec3 v4;
    max_dist = 0;

    // Finds fourth point that create largest overall tetrahedron
    for (auto v = verts.begin(); v != verts.end(); v++) {
        float dist = dot(norm, *v - v1);
        if (abs(max_dist) < abs(dist)) {
            max_dist = dist;
            v4 = *v;
        }
    }

    Triangle t1, t2, t3, t4;

    if (max_dist < 0) {
        t1 = Triangle(v1, v2, v3, verts);
    }
    else {
        t1 = Triangle(v1, v3, v2, verts);
    }

    t2 = Triangle::create_triangle(t1, v1, v2, v4, verts, false);
    t3 = Triangle::create_triangle(t1, v1, v3, v4, verts, false);
    t4 = Triangle::create_triangle(t1, v2, v3, v4, verts, false);

    vector<vec3> final_verts;
    vector<GLuint> indices;
    stack<Triangle> tris;
    tris.push(t1);
    tris.push(t2);
    tris.push(t3);
    tris.push(t4);

    while (!tris.empty()) {
        Triangle t = tris.top();
        tris.pop();

        if (t.is_edge()) {
            final_verts.push_back(t.v1);
            final_verts.push_back(t.v2);
            final_verts.push_back(t.v3);

            indices.push_back(indices.size());
            indices.push_back(indices.size());
            indices.push_back(indices.size());
        }
        else {
            vec3 v = t.max_point();
            t1 = Triangle::create_triangle(t, t.v1, t.v2, v, t.verts);
            t2 = Triangle::create_triangle(t, t.v1, v, t.v3, t.verts);
            t3 = Triangle::create_triangle(t, v, t.v2, t.v3, t.verts);

            tris.push(t1);
            tris.push(t2);
            tris.push(t3);
        }
    }

    Mesh temp;
    temp.verts = final_verts;
    temp.indices = indices;
    temp.norms = vector<vec3>(1);

    return temp;
}