#include "Triangle.h"

Triangle::Triangle() {}

Triangle::Triangle(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
    this->material = Material(rand()%256, rand()%256, rand()%256);
}

CollisionData Triangle::intersect(const Ray &ray) const {
    //M�ller�Trumbore intersection algorithm
    CollisionData data;

    const Vector3 edge1 = v1 - v0;
    const Vector3 edge2 = v2 - v0;
    const Vector3 h = ray.direction^edge2;

    double a = edge1*h;
    if (a>-EPSILON && a<EPSILON)
        return data;    // This ray is parallel to this triangle.

    double f = 1.0/a;
    Vector3 s = ray.origin - v0;
    double u = f * (s*h);
    if(u<0.0 || u>1.0)
        return data;

    Vector3 q = s^edge1;
    double v = f*(ray.direction*q);
    if(v<0.0 || u+v>1.0)
        return data;

    double t = f*(edge2*q);
    if(t>EPSILON) {
        data.point = ray.origin + t*normalize(ray.direction);
        data.normal = normalize(edge1^edge2);
        data.color = this->material.getColorAt(0, 0);
        data.material = this->material;
        data.distance = t;
        data.exist = true;
        return data;
    }

    // This means that there is a line intersection but not a ray intersection.
    data.exist = false;
    return data;
}
