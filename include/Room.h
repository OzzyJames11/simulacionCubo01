// include/Room.h
#pragma once
#include "Plane.h"

class Room {
public:
    Plane  faces[6];   // 0 piso, 1 techo, 2 izq, 3 der, 4 fondo, 5 frente
    double half;

    Room();
    Room(double side);

    // Primera pared que toca el rayo (menor t > 0). Devuelve false si no toca ninguna.
    bool firstHit(Point o, Vector d, Point& hit, int* faceIdx = nullptr);
};