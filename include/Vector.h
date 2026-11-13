#pragma once

class Vector {
public:
    double x, y, z;

    Vector();
    Vector(double x, double y, double z);

    double operator*(Vector v);     // producto punto
    Vector operator/(Vector v);     // producto cruz
    Vector operator*(double k);     // escalar
    Vector operator+(Vector v);
    Vector operator-(Vector v);

    double length();
    Vector normalized();            // vector unitario (devuelve 0 si |v| ≈ 0)
    Vector reflect(Vector n);       // reflexión de *this sobre una pared de normal n
};