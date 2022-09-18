#pragma once

#include <algorithm>
#include "rtrace.h"

/*  User custom Torus class
    It is drawable object looks like donut.    */

class Torus : public rtrace::Object {
private:
    double majorRadius, minorRadius;    // small and big radius

	rtrace::Vector3 getNormal(const rtrace::Vector3&) const;
public:
    Torus(const rtrace::Vector3&, const double&, const double&, const rtrace::Material&);

    rtrace::CollisionData intersect(const rtrace::Ray&) const;      // method overrite for ray tracing
    rtrace::CollisionData distance(const rtrace::Vector3&) const;   // method override for sphere tracing
};
