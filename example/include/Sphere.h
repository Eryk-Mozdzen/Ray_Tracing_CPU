#ifndef SPHERE_H
#define SPHERE_H

#include "RayTracing.h"

class Sphere : public Object {
private:
    double radius;
    Material material;
    Transform3 transform;
public:
    Sphere();
    Sphere(const Vector3&, const double&);
    Sphere(const Vector3&, const double&, const Material&);

    sf::Color getPixel(const Vector3&) const;
    const Transform3 & getTransform() const;
    const Material & getMaterial() const;

    void setTransform(const Transform3&);
    void setMaterial(const Material&);

    CollisionData intersect(const Ray&) const;
    CollisionData distance(const Vector3&) const;
};

#endif