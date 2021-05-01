#include "Box.h"

Vector3 Box::min(const Vector3 &v1, const Vector3 &v2) const {
    return Vector3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
}

Vector3 Box::max(const Vector3 &v1, const Vector3 &v2) const {
    return Vector3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
}

Vector3 Box::abs(const Vector3 &v) const {
    return Vector3(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

Box::Box() {
    this->dimensions = Vector3(1, 1, 1);
}

Box::Box(const Vector3 &position, const Vector3 &dimensions, const Material &material) {
    this->transform.translate(position);
    this->dimensions = dimensions;
    this->material = material;
}

CollisionData Box::distance(const Vector3 &point) const {
    CollisionData data;

    //const Vector3 q = length(point) - this->transform.getTranslation();
    data.distance = 1;//length(this->max(q, Vector3(0, 0, 0))) + std::min(std::max(q.x, std::max(q.y, q.z)), 0.);

    data.point = point;
    data.color = this->material.getColor();
    data.exist = (data.distance<EPSILON);

    return data;
}
