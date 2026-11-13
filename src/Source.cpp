// src/Source.cpp
#include "Source.h"
#include <cmath>

static const double PI_D = 3.14159265358979323846;

Source::Source() {
    eF = 0.0;
    NRAYS = 0;
    Rays = nullptr;
}

Source::~Source() {
    delete[] Rays;
}

void Source::createRays(double NumberOfRays) {
    // Aristas {1er punto, 2o punto, posición en Rays (se rellena abajo)}
    int A[30][3] = {
        {0,1,0},  {0,2,0},  {0,3,0},  {0,4,0},  {0,5,0},
        {1,6,0},  {2,6,0},  {2,7,0},  {3,7,0},  {3,8,0},
        {4,8,0},  {4,9,0},  {5,9,0},  {5,10,0}, {1,10,0},
        {6,11,0}, {7,11,0}, {8,11,0}, {9,11,0}, {10,11,0},
        {1,2,0},  {2,3,0},  {3,4,0},  {4,5,0},  {5,1,0},
        {6,7,0},  {7,8,0},  {8,9,0},  {9,10,0}, {10,6,0}
    };
    // Triángulos {1a arista, 2a arista, [0] V en pie, [-1] V invertida}
    int T[20][3] = {
        {0,1,0},    {1,2,0},    {2,3,0},    {3,4,0},    {4,0,0},
        {5,6,-1},   {6,7,0},    {7,8,-1},   {8,9,0},    {9,10,-1},
        {10,11,0},  {11,12,-1}, {12,13,0},  {13,14,-1}, {14,5,0},
        {15,16,-1}, {16,17,-1}, {17,18,-1}, {18,19,-1}, {19,15,-1}
    };

    int i, j, k, n, m, RAY;
    double S, R, xB, yB, zB, xC, yC, zC, c[8];

    if (NRAYS > 0) delete[] Rays;
    n = int(floor(sqrt((NumberOfRays - 2) / 10) + 0.5));
    NRAYS = int(2 + 10 * pow(n, 2));
    Rays = new Vector[NRAYS];

    // Vértices del icosaedro
    S = 2 / sqrt(5.0);
    R = (5 - sqrt(5.0)) / 5;
    Rays[0].x = 0; Rays[0].y = 0; Rays[0].z = 1;
    for (i = 1; i < 6; i++) {
        Rays[i].x   = S * cos((PI_D * i * 72) / 180);
        Rays[i].y   = S * sin((PI_D * i * 72) / 180);
        Rays[i].z   = 1 - R;
        Rays[i+5].x = S * cos((72 * PI_D * i) / 180 + (36 * PI_D) / 180);
        Rays[i+5].y = S * sin((72 * PI_D * i) / 180 + (36 * PI_D) / 180);
        Rays[i+5].z = R - 1;
    }
    Rays[11].x = 0; Rays[11].y = 0; Rays[11].z = -1;
    RAY = 12;

    // Rayos sobre las aristas
    for (j = 0; j < 30; j++) {
        A[j][2] = RAY;
        xB = Rays[A[j][0]].x; yB = Rays[A[j][0]].y; zB = Rays[A[j][0]].z;
        xC = Rays[A[j][1]].x; yC = Rays[A[j][1]].y; zC = Rays[A[j][1]].z;
        c[0] = pow(xC,2)*(pow(yB,2)+pow(zB,2)) + pow(yC*zB-yB*zC,2)
             - 2*xB*xC*(yB*yC+zB*zC) + pow(xB,2)*(pow(yC,2)+pow(zC,2));
        c[1] = acos(xB*xC + yB*yC + zB*zC);
        c[2] = -xC*(yB*yC+zB*zC) + xB*(pow(yC,2)+pow(zC,2));
        c[3] = xC*(pow(yB,2)+pow(zB,2)) - xB*(yB*yC+zB*zC);
        c[4] = pow(xC,2)*yB - xB*xC*yC + zC*(-yC*zB+yB*zC);
        c[5] = -xB*xC*yB + pow(xB,2)*yC + zB*(yC*zB-yB*zC);
        c[6] = pow(xC,2)*zB - xB*xC*zC + yC*(yC*zB-yB*zC);
        c[7] = -xB*xC*zB + pow(xB,2)*zC + yB*(-yC*zB+yB*zC);
        for (i = 1; i < n; i++) {
            Rays[RAY].x = (c[2]*cos(i*c[1]/n) + c[3]*cos((n-i)*c[1]/n)) / c[0];
            Rays[RAY].y = (c[4]*cos(i*c[1]/n) + c[5]*cos((n-i)*c[1]/n)) / c[0];
            Rays[RAY].z = (c[6]*cos(i*c[1]/n) + c[7]*cos((n-i)*c[1]/n)) / c[0];
            RAY++;
        }
    }

    // Rayos sobre las caras
    for (k = 0; k < 20; k++)
        for (j = 1; j < n; j++) {
            xB = Rays[A[T[k][0]][2]+j-1].x;
            yB = Rays[A[T[k][0]][2]+j-1].y;
            zB = Rays[A[T[k][0]][2]+j-1].z;
            xC = Rays[A[T[k][1]][2]+j-1].x;
            yC = Rays[A[T[k][1]][2]+j-1].y;
            zC = Rays[A[T[k][1]][2]+j-1].z;
            c[0] = pow(xC,2)*(pow(yB,2)+pow(zB,2)) + pow(yC*zB-yB*zC,2)
                 - 2*xB*xC*(yB*yC+zB*zC) + pow(xB,2)*(pow(yC,2)+pow(zC,2));
            c[1] = acos(xB*xC + yB*yC + zB*zC);
            c[2] = -xC*(yB*yC+zB*zC) + xB*(pow(yC,2)+pow(zC,2));
            c[3] = xC*(pow(yB,2)+pow(zB,2)) - xB*(yB*yC+zB*zC);
            c[4] = pow(xC,2)*yB - xB*xC*yC + zC*(-yC*zB+yB*zC);
            c[5] = -xB*xC*yB + pow(xB,2)*yC + zB*(yC*zB-yB*zC);
            c[6] = pow(xC,2)*zB - xB*xC*zC + yC*(yC*zB-yB*zC);
            c[7] = -xB*xC*zB + pow(xB,2)*zC + yB*(-yC*zB+yB*zC);
            if (T[k][2] == 0) m = j;
            else              m = n - j;
            for (i = 1; i < m; i++) {
                Rays[RAY].x = (c[2]*cos(i*c[1]/m) + c[3]*cos((m-i)*c[1]/m)) / c[0];
                Rays[RAY].y = (c[4]*cos(i*c[1]/m) + c[5]*cos((m-i)*c[1]/m)) / c[0];
                Rays[RAY].z = (c[6]*cos(i*c[1]/m) + c[7]*cos((m-i)*c[1]/m)) / c[0];
                RAY++;
            }
        }
}