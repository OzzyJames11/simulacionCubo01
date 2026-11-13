// include/Intersect.h
#pragma once
#include "Point.h"
#include "Vector.h"
#include "Plane.h"

// Intersección rayo-plano. Devuelve true si el rayo corta el plano delante del origen.
// t = parámetro escalar, pInt = punto 3D de intersección (O + t*d).
bool intersectRayPlane(Point o, Vector d, Plane pl, double& t, Point& pInt);