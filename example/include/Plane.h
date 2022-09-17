#pragma once

#include "RayTracing.h"

/*  User custom Plane class
    It is drawable object looks like infinity plane, flat ground.    */

class Plane : public Object {
private:
    double A, B, C, D;      // mathematic definition of a plane
    Material material;      // material used to define plane texture
public:
    Plane(const Vector3&, const Vector3&);
    Plane(const Vector3&, const Vector3&, const Material&);

    sf::Color getPixel(const Vector3&) const;
    Vector3 getNormal() const;

    CollisionData intersect(const Ray&) const;      // method overrite for ray tracing
    CollisionData distance(const Vector3&) const;   // method override for sphere tracing
};
