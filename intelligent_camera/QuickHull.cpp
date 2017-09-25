#include "QuickHull.h"

#include <algorithm>
#include <climits>
#include <list>
#include <numeric>
#include <unordered_set>
#include <stack>
#include <vector>

#include <glm\glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\hash.hpp>

#include <engine_base\Mesh.h>

#include "Globals.h"

using namespace glm;
using namespace std;

class Edge {
	vec3 _start, _end;
public:
	Edge() {}

	Edge(vec3 start, vec3 end) {
		_start = start;
		_end = end;
	}

	~Edge() {}

	bool operator== (const Edge& e) const {
		return (_start == e._start && _end == e._end) || (_start == e._end && _end == e._start);
	}

	vec3 start() const {
		return _start;
	}

	vec3 end() const {
		return _end;
	}
};

class Triangle {
	Edge _e1, _e2, _e3;
	vec3 _norm;
    vec3 _center;
	vector<vec3> _verts;

public:
	Triangle() {}

	Triangle(Edge e1, Edge e2, Edge e3, vector<vec3>& verts) {
		_e1 = e1;
		_e2 = e2;
		_e3 = e3;

#ifdef _DEBUG
		assert(_e1.end() == _e2.start());
		assert(_e2.end() == _e3.start());
		assert(_e3.end() == _e1.start());
#endif
        _center = (_e1.start() + _e2.start() + _e3.start()) / 3.f;
        _norm = normalize(cross(_e2.start() - _e1.start(), _e3.start() - _e1.start()));

		verts.erase(remove(verts.begin(), verts.end(), _e1.start()), verts.end());
        verts.erase(remove(verts.begin(), verts.end(), _e2.start()), verts.end());
        verts.erase(remove(verts.begin(), verts.end(), _e3.start()), verts.end());

        for (auto v : verts) {
            if (can_see(v)) {
                _verts.push_back(v);
            }
        }
        
        for (auto v : _verts) {
            verts.erase(remove(verts.begin(), verts.end(), v));
        }
	}

	~Triangle() {}

	bool operator== (const Triangle& t) const {
		return _e1 == t._e1 && _e2 == t._e2 && _e3 == t._e3;
	}

	bool can_see(vec3& pt) {
		vec3 pt_ray = pt - _e1.start();
        bool res = dot(_norm, pt_ray) > 0;
        return res;
	}

	bool on_hull() {
		return _verts.empty();
	}

	Edge e1() {
		return _e1;
	}

	Edge e2() {
		return _e2;
	}

	Edge e3() {
		return _e3;
	}

    vec3 center() {
        return _center;
    }

	vec3 max_point() {
		vec3 pt;
		int max_dist = -1;
		for (auto v = _verts.begin(); v != _verts.end(); v++) {
			float dist = dot(_norm, pt - _center);
			if (max_dist < dist) {
				max_dist = dist;
				pt = *v;
			}
		}
		return pt;
	}

	vector<vec3>& verts() {
		return _verts;
	}

	static Triangle create_triangle(vec3 center, vec3 p1, vec3 p2, vec3 p3, vector<vec3> & verts) {
		vec3 tri_center = (p1 + p2 + p3) / 3.f;
        vec3 center_ray = tri_center - center;
        vec3 norm = normalize(cross(p2 - p1, p3 - p1));

		Edge e1, e2, e3;
		if (dot(norm, center_ray) > 0) {
			e1 = Edge(p1, p2);
			e2 = Edge(p2, p3);
			e3 = Edge(p3, p1);
		}
		else {
			e1 = Edge(p1, p3);
			e2 = Edge(p3, p2);
			e3 = Edge(p2, p1);
		}

		return Triangle(e1, e2, e3, verts);
	}
};

QuickHull::QuickHull() {}

QuickHull::~QuickHull() {}

Mesh QuickHull::GenerateHull(const Mesh & mesh) {

	if (mesh.verts.size() <= 4) return mesh;

    unordered_set<vec3> temp = unordered_set<vec3>(mesh.verts.begin(), mesh.verts.end());
	vector<vec3> verts = vector<vec3>(temp.begin(), temp.end());

	vec3 p1, p2;
	float max_dist = -1;

	// Finds furthest apart extremes
	for (auto i = verts.begin(); i != verts.end(); i++) {
		for (auto j = i + 1; j != verts.end(); j++) {
			float dist = abs(distance(*i, *j));
			if (max_dist < dist) {
				max_dist = dist;
				p1 = *i;
				p2 = *j;
			}
		}
	}

	vec3 p3;
	max_dist = -1;

	// Finds extreme that creates the largest triangle with the previous two
	for (auto v = verts.begin(); v != verts.end(); v++) {
		float dist = abs(length(cross(*v - p1, *v - p2)) / length(p2 - p1));
		if (max_dist < dist) {
			max_dist = dist;
			p3 = *v;
		}
	}

	vec3 norm = normalize(cross(p3 - p1, p2 - p1));

	vec3 p4;
	max_dist = 0;

	// Finds fourth point that create largest overall tetrahedron
	for (auto v = verts.begin(); v != verts.end(); v++) {
		float dist = dot(norm, *v - p1);
		if (abs(max_dist) < abs(dist)) {
			max_dist = dist;
			p4 = *v;
		}
	}

	vec3 center = (p1 + p2 + p3 + p4) / 4.f;

	vector<Triangle> tris;
	stack<Triangle> worklist;

	Triangle t1, t2, t3, t4;

	t1 = Triangle::create_triangle(center, p1, p2, p3, verts);
	t2 = Triangle::create_triangle(center, p1, p2, p4, verts);
	t3 = Triangle::create_triangle(center, p1, p3, p4, verts);
	t4 = Triangle::create_triangle(center, p2, p3, p4, verts);

	worklist.push(t1);
	tris.push_back(t1);
	worklist.push(t2);
	tris.push_back(t2);
	worklist.push(t3);
	tris.push_back(t3);
	worklist.push(t4);
	tris.push_back(t4);

    int counter = hull_counter;
	while (!worklist.empty()) {
		Triangle t = worklist.top();
		worklist.pop();

		if (t.on_hull()) continue;
        counter--;

		vec3 pt = t.max_point();

        vector<Triangle> to_remove;
        vector<Edge> can_see, not_see;
		for (auto tri : tris) {
			if (tri.can_see(pt)) {
				can_see.push_back(tri.e1());
				can_see.push_back(tri.e2());
				can_see.push_back(tri.e3());
                to_remove.push_back(tri);
			}
			else {
				not_see.push_back(tri.e1());
				not_see.push_back(tri.e2());
				not_see.push_back(tri.e3());
			}
		}

        vector<Edge> horizon;
        for (auto e : can_see) {
            if (find(not_see.begin(), not_see.end(), e) != not_see.end())
                horizon.push_back(e);
        }

		for (auto h : horizon) {
			Triangle new_t = Triangle::create_triangle(center, h.start(), h.end(), pt, t.verts());
			worklist.push(new_t);
			tris.push_back(new_t);
		}

        for (auto tri : to_remove) {
            tris.erase(remove(tris.begin(), tris.end(), tri));
        }
	}


	vector<vec3> final_verts;
	vector<uint> indices;
	for (auto t : tris) {
		final_verts.push_back(t.e1().start());
		final_verts.push_back(t.e2().start());
		final_verts.push_back(t.e3().start());

		indices.push_back(indices.size());
		indices.push_back(indices.size());
		indices.push_back(indices.size());
	}

	Mesh hull;
    hull.verts = final_verts;
    hull.indices = indices;

	return hull;
}