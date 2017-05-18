#pragma once

class Mesh;

class QuickHull {
public:
    QuickHull();
    ~QuickHull();

    static Mesh GenerateHull(const Mesh& mesh);
};

