// src/Room.cpp
#include "Room.h"
#include "Intersect.h"

Room::Room() : half(0.0) {}

Room::Room(double side) {
    double h = side / 2.0;
    half = h;

    // Cada cara: vértices antihorarios vistos desde el INTERIOR.
    // Plane calcula la normal con (b-a) / (d-a).

    // Piso (y=-h), normal +Y
    faces[0] = Plane(Point(-h,-h,-h), Point(-h,-h, h), Point( h,-h, h), Point( h,-h,-h));
    // Techo (y=+h), normal -Y
    faces[1] = Plane(Point(-h, h,-h), Point( h, h,-h), Point( h, h, h), Point(-h, h, h));
    // Izquierda (x=-h), normal +X
    faces[2] = Plane(Point(-h,-h,-h), Point(-h, h,-h), Point(-h, h, h), Point(-h,-h, h));
    // Derecha (x=+h), normal -X
    faces[3] = Plane(Point( h,-h,-h), Point( h,-h, h), Point( h, h, h), Point( h, h,-h));
    // Fondo (z=-h), normal +Z
    faces[4] = Plane(Point(-h,-h,-h), Point( h,-h,-h), Point( h, h,-h), Point(-h, h,-h));
    // Frente (z=+h), normal -Z
    faces[5] = Plane(Point(-h,-h, h), Point(-h, h, h), Point( h, h, h), Point( h,-h, h));
}

bool Room::firstHit(Point o, Vector d, Point& hit, int* faceIdx) {
    bool found = false;
    double bestT = 0.0;

    for (int f = 0; f < 6; f++) {
        double t;
        Point p;
        if (intersectRayPlane(o, d, faces[f], t, p)) {
            if (!found || t < bestT) {
                found = true;
                bestT = t;
                hit = p;
                if (faceIdx) *faceIdx = f;
            }
        }
    }
    return found;
}