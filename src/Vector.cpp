#include "Vector.h"
#include <cmath>

Vector::Vector() { x = y = z = 0.0; }
Vector::Vector(double X, double Y, double Z) { x = X; y = Y; z = Z; }

double Vector::operator*(Vector v) { return (x*v.x) + (y*v.y) + (z*v.z); }

Vector Vector::operator/(Vector v) {
    Vector r;
    r.x = (y*v.z) - (z*v.y);
    r.y = (z*v.x) - (x*v.z);
    r.z = (x*v.y) - (y*v.x);
    return r;
}

Vector Vector::operator*(double k) { return Vector(x*k, y*k, z*k); }
Vector Vector::operator+(Vector v) { return Vector(x+v.x, y+v.y, z+v.z); }
Vector Vector::operator-(Vector v) { return Vector(x-v.x, y-v.y, z-v.z); }

double Vector::length() { return std::sqrt(x*x + y*y + z*z); }

Vector Vector::normalized() {
    double l = length();
    if (l < 1e-12) return Vector();
    return Vector(x/l, y/l, z/l);
}

// R = I - 2 (I · N̂) N̂
Vector Vector::reflect(Vector n) {
    Vector nu = n.normalized();
    double k = 2.0 * ((*this) * nu);
    return (*this) - nu * k;
}