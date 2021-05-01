#ifndef BOX_H
#define BOX_H

#include "RayTracing.h"

class Box : public Object {
private:
    Vector3 dimensions;
    Transform3 transform;
    Material material;

    Vector3 min(const Vector3 &v1, const Vector3 &v2) const;
    Vector3 max(const Vector3 &v1, const Vector3 &v2) const;
    Vector3 abs(const Vector3 &v) const;
public:
    Box();
    Box(const Vector3 &position, const Vector3 &dimensions, const Material &material);

    CollisionData distance(const Vector3 &point) const;
};

#endif