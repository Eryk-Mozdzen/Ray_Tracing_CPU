#ifndef TORUS_H
#define TORUS_H

#include "RayTracing.h"

class Torus : public Object {
private:
    double majorRadius, minorRadius;
    Material material;
    Transform3 transform;
public:
    Torus();
    Torus(const Vector3&, const double&, const double&);
    Torus(const Vector3&, const double&, const double&, const Material&);

    sf::Color getPixel() const;
    Vector3 getNormal(const Vector3&) const;

    CollisionData intersect(const Ray&) const;
    CollisionData distance(const Vector3&) const;
};

#endif