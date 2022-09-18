#pragma once

#include "rtrace.h"

/*  User custom Plane class
    It is drawable object looks like infinity plane, flat ground.    */

class Plane : public Object {
private:
    double A, B, C, D;      // mathematic definition of a plane
	
	Vector3 getNormal() const;
	sf::Color getPixel(const Vector3&) const;
public:
    Plane(const Vector3&, const Vector3&, const Material&);

    CollisionData intersect(const Ray&) const;      // method overrite for ray tracing
    CollisionData distance(const Vector3&) const;   // method override for sphere tracing
};
