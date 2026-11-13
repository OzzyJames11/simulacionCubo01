#include "Point.h"

Point::Point() { x = y = z = 0.0; }

// Implementación del nuevo constructor
Point::Point(double _x, double _y, double _z) {
    x = _x;
    y = _y;
    z = _z;
}

Vector Point::operator-(Point p) {
    Vector r;
    r.x = x - p.x;
    r.y = y - p.y;
    r.z = z - p.z;
    return r;
}
