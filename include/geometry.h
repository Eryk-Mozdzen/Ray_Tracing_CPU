#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <string>
#include <cmath>
#include "Vector.h"

class Line {
public:
    Vector3 point;
    Vector3 direction;

    Line();
    Line(const Vector3&, const Vector3&);
};

class Plane {
public:
    double A, B, C, D;

    Plane();
    Plane(const double&, const double&, const double&, const double&);
    Plane(const Vector3&, const Vector3&);
    Plane(const Vector3&, const Vector3&, const Vector3&);
    Vector3 getNormal() const;
    bool intersect(const Line&, Vector3*) const;
};

double distance(const Line&, const Vector3&);
Matrix solveLinearSystem(const Matrix&, const Matrix&);

#endif
