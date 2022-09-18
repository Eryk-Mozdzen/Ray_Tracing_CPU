#pragma once

#include <algorithm>
#include "rtrace.h"

/*  User custom Torus class
    It is drawable object looks like donut.    */

class Torus : public Object {
private:
    double majorRadius, minorRadius;    // small and big radius

	Vector3 getNormal(const Vector3&) const;
public:
    Torus(const Vector3&, const double&, const double&, const Material&);

    CollisionData intersect(const Ray&) const;      // method overrite for ray tracing
    CollisionData distance(const Vector3&) const;   // method override for sphere tracing
};
