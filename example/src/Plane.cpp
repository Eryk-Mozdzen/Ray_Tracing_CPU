#include "Plane.h"

Plane::Plane(const Vector3 &point, const Vector3 &normal) {
    Vector3 norm = normalize(normal);
    A = norm.x;
    B = norm.y;
    C = norm.z;
    D = -A*point.x - B*point.y - C*point.z;

    this->material.setAmbient(0);
    this->material.setDiffuse(1);
    this->material.setSpecular(0);
    this->material.setShininess(1);
    this->material.setReflection(0);
}

Plane::Plane(const Vector3 &point, const Vector3 &normal, const Material &material) : Plane(point, normal) {
    this->material = material;
    this->material.setAmbient(0);
    this->material.setDiffuse(1);
    this->material.setSpecular(0);
    this->material.setShininess(1);
    this->material.setReflection(0.5);
}

sf::Color Plane::getPixel(const Vector3 &point3D) const {
    //only special case
    sf::Vector2f point2D(point3D.x, point3D.y);

    return this->material.getColorAt(point3D.x, point3D.y);
}

Vector3 Plane::getNormal() const {
    return Vector3(this->A, this->B, this->C);
}

CollisionData Plane::intersect(const Ray &ray) const {
    CollisionData data;
    const double dot = this->getNormal()*ray.direction;
    if(dot<EPSILON && dot>-EPSILON)
        return data;

    Vector3 p0 = Vector3(0, 0, -D/C);
    Vector3 p1 = Vector3(1, 0, -(D + A)/C);
    Vector3 p2 = Vector3(1, 1, -(D + A + B)/C);

    Vector3 p01 = p1 - p0;
    Vector3 p02 = p2 - p0;

    double t = -(((p01^p02)*(ray.origin - p0))/(ray.direction*(p01^p02)));

    data.point = ray.origin + t*ray.direction;

    if((ray.direction*(data.point - ray.origin))<EPSILON)
        return data;

    data.normal = this->getNormal();
    data.color = this->getPixel(data.point);
    data.material = this->material;
    data.distance = length(data.point - ray.origin);
    data.exist = true;
    return data;
}

CollisionData Plane::distance(const Vector3 &point) const {
    CollisionData data;
    const Vector3 P(0, 0, -this->D/this->C);
    const Vector3 N = this->getNormal();
    const Vector3 V = point - P;

    data.distance = std::abs(V*N);
    data.point = point;
    data.normal = N;
    data.material = this->material;
    data.color = this->getPixel(point + N*data.distance);
    data.exist = (data.distance<EPSILON);

    return data;
}
