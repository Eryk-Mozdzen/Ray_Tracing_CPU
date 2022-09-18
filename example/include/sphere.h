#pragma once

#include "rtrace.h"

/*  User custom Sphere class
    It is drawable object looks like ball. Can have color or texture.    */

class Sphere : public rtrace::Object {
private:
    double radius;          // radius
	
	sf::Color getPixel(const rtrace::Vector3&) const;
public:
    Sphere(const rtrace::Vector3&, const double&, const rtrace::Material&);

    rtrace::CollisionData intersect(const rtrace::Ray&) const;      // method overrite for ray tracing
    rtrace::CollisionData distance(const rtrace::Vector3&) const;   // method override for sphere tracing
};