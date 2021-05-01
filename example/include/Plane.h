#ifndef PLANE_H
#define PLANE_H

#include "RayTracing.h"

class Plane : public Object {
private:
    double A, B, C, D;
    Material material;
public:
    Plane(const Vector3&, const Vector3&);
    Plane(const Vector3&, const Vector3&, const Material&);

    sf::Color getPixel(const Vector3&) const;
    Vector3 getNormal() const;

    CollisionData intersect(const Ray&) const;
    CollisionData distance(const Vector3&) const;
};


#endif