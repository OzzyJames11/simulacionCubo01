#pragma once
#include "Point.h"
#include "Vector.h"

class Plane {
public:
    Point v[4];     // vértices en orden antihorario visto desde el interior
    Vector normal;  // normal hacia el interior de la sala (sin normalizar)

    Plane();
    Plane(Point a, Point b, Point c, Point d);

    Point  center();       // centro de la cara
    Vector unitNormal();   // normal de longitud 1
};