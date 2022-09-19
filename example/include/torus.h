#pragma once

#include <algorithm>
#include "object.h"

/*  User custom Torus class
    It is drawable object looks like donut.    */

class Torus : public rtrace::Object {
private:
    double majorRadius, minorRadius;

	rtrace::Vector3 getNormal(const rtrace::Vector3&) const;
public:
    Torus(const rtrace::Vector3&, const double&, const double&, const rtrace::Material&);

    rtrace::Collision intersect(const rtrace::Ray&) const;
    rtrace::Collision distance(const rtrace::Vector3&) const;
};
