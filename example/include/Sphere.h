#pragma once

#include "RayTracing.h"

/*  User custom Sphere class
    It is drawable object looks like ball. Can have color or texture.    */

class Sphere : public Object {
private:
    double radius;          // radius
    Material material;      // material (for color or texture)
    Transform3 transform;   // transform matrix (position and rotation)
public:
    Sphere(const Vector3&, const double&, const Material&);

    sf::Color getPixel(const Vector3&) const;
    const Transform3 & getTransform() const;
    const Material & getMaterial() const;

    void setTransform(const Transform3&);
    void setMaterial(const Material&);

    CollisionData intersect(const Ray&) const;      // method overrite for ray tracing
    CollisionData distance(const Vector3&) const;   // method override for sphere tracing
};
