#pragma once

#include "RayTracing.h"

/*  User custom Sphere class
    It is drawable object looks like ball. Can have color or texture.    */

class Sphere : public Object {
private:
    double radius;          // radius
	
	sf::Color getPixel(const Vector3&) const;
public:
    Sphere(const Vector3&, const double&, const Material&);

    CollisionData intersect(const Ray&) const;      // method overrite for ray tracing
    CollisionData distance(const Vector3&) const;   // method override for sphere tracing
};
