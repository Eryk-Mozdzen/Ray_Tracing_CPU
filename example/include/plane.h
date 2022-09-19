#pragma once

#include "rtrace.h"

/*  User custom Plane class
    It is drawable object looks like infinity plane, flat ground.    */

class Plane : public rtrace::Object {
private:
    double A, B, C, D;
	
	rtrace::Vector3 getNormal() const;
public:
    Plane(const rtrace::Vector3&, const rtrace::Vector3&, const rtrace::Material&);

    rtrace::CollisionData intersect(const rtrace::Ray&) const;
    rtrace::CollisionData distance(const rtrace::Vector3&) const;
};
