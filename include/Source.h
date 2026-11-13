// include/Source.h
#pragma once
#include "Point.h"
#include "Vector.h"

class Source {
public:
    Point  p;        // Posición
    Vector *Rays;    // Direcciones de partida de los rayos
    int    NRAYS;    // Número de rayos
    double eF;       // Energía inicial de la fuente

    Source();
    ~Source();
    Source(const Source&) = delete;
    Source& operator=(const Source&) = delete;

    void createRays(double NumberOfRays);
};