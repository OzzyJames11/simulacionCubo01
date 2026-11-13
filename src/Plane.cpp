#include "Plane.h"

Plane::Plane() {}

Plane::Plane(Point a, Point b, Point c, Point d) {
    v[0] = a; v[1] = b; v[2] = c; v[3] = d;

    Vector e1 = b - a;
    Vector e2 = d - a;
    normal = e1 / e2;    // producto cruz con el operador /
}

Point Plane::center() {
    return Point((v[0].x + v[1].x + v[2].x + v[3].x) / 4.0,
                 (v[0].y + v[1].y + v[2].y + v[3].y) / 4.0,
                 (v[0].z + v[1].z + v[2].z + v[3].z) / 4.0);
}

Vector Plane::unitNormal() {
    return normal.normalized();
}