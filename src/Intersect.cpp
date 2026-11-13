// src/Intersect.cpp
#include "Intersect.h"
#include <cmath>

bool intersectRayPlane(Point o, Vector d, Plane pl, double& t, Point& pInt) {
    const double EPS = 1e-9;

    double denom = pl.normal * d;            // n · d (producto punto)
    if (std::fabs(denom) < EPS) return false; // rayo paralelo al plano

    Vector w = pl.v[0] - o;                  // v0 - O
    t = (pl.normal * w) / denom;
    if (t < EPS) return false;               // plano detrás (o sobre) el origen

    pInt = Point(o.x + t * d.x, o.y + t * d.y, o.z + t * d.z);
    return true;
}