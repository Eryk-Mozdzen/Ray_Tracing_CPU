#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "RayTracing.h"

class Triangle : public Object {
private:
    Vector3 v0, v1, v2;
    Material material;
public:
    Triangle();
    Triangle(const Vector3&, const Vector3&, const Vector3&);

    CollisionData intersect(const Ray&) const;
};

#endif