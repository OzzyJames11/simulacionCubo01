#pragma once
#include "Vector.h"

class Point {
public:
    double x, y, z;
    Point();
    Point(double _x, double _y, double _z); // Nuevo constructor
    Vector operator-(Point p);
};
