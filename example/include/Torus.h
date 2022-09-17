#pragma once

#include "RayTracing.h"

/*  User custom Torus class
    It is drawable object looks like donut.    */

class Torus : public Object {
private:
    double majorRadius, minorRadius;    // small and big radius
    Material material;                  // materal (for color)
    Transform3 transform;               // transform matrix (for position)
public:
    Torus();
    Torus(const Vector3&, const double&, const double&);
    Torus(const Vector3&, const double&, const double&, const Material&);

    sf::Color getPixel() const;
    Vector3 getNormal(const Vector3&) const;

    //CollisionData intersect(const Ray&) const;      // method overrite for ray tracing (this one is not well defined, so I leave this)
    CollisionData distance(const Vector3&) const;   // method override for sphere tracing
};
