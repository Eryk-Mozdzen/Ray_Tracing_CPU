#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cmath>
#include "Vector.h"

class Line {
public:
    Vector3 point;
    Vector3 direction;

    Line();
    Line(Vector3, Vector3);
};

class Plane {
public:
    double A, B, C, D;

    Plane();
    Plane(double, double, double, double);
    Plane(Vector3, Vector3);
    Plane(Vector3, Vector3, Vector3);
    Vector3 getNormal();
    bool intersect(Line, Vector3*);
};

double distance(Line, Vector3);

#endif
