#include "plane.h"

Plane::Plane(const rtrace::Vector3 &point, const rtrace::Vector3 &normal, const rtrace::Material &material) {
	const rtrace::Vector3 norm = rtrace::normalize(normal);
    this->A = norm.x;
    this->B = norm.y;
    this->C = norm.z;
    this->D = -A*point.x - B*point.y - C*point.z;

	this->material = material;
	this->material.ambient = 0;
	this->material.diffuse = 1;
	this->material.specular = 0;
	this->material.shininess = 1;
	this->material.reflection = 0.5;
}

rtrace::Vector3 Plane::getNormal() const {
    return rtrace::Vector3(this->A, this->B, this->C);
}

/*  CollisionData CustomObject::intersect(const Ray &) const

    method parameter is Ray struct, 
    if user objects collide with that ray, should return correct CollisionData struct
    if not, should return not changed CollisionData struct  */

rtrace::Collision Plane::intersect(const rtrace::Ray &ray) const {
    rtrace::Collision data;     // construct fills struct with correct fields

    const double dot = this->getNormal()*ray.direction;
    if(dot<rtrace::EPSILON && dot>-rtrace::EPSILON)
        return data;        // return not changed if ray parallel to plane

    rtrace::Vector3 p0(0, 0, -D/C);
    rtrace::Vector3 p1(1, 0, -(D + A)/C);
    rtrace::Vector3 p2(1, 1, -(D + A + B)/C);

    rtrace::Vector3 p01 = p1 - p0;
    rtrace::Vector3 p02 = p2 - p0;

    double t = -(((p01^p02)*(ray.origin - p0))/(ray.direction*(p01^p02)));

    data.point = ray.origin + t*ray.direction;          // set collision point

    if((ray.direction*(data.point - ray.origin))<rtrace::EPSILON)
        return data;        // return not changed if result point of collision is behind ray origin

    // if ray collide with object fill data fields in correct way
    data.normal = this->getNormal();                    // set surface notrmal vector in collision point
    data.material = this->material;                     // set material of the objects
    data.distance = rtrace::length(data.point - ray.origin);    // set distance from ray origin to collision point
    data.exist = true;                                  // collision occured? set to true

    return data;
}

/*  CollisionData CustomObject::distance(const Vector3 &) const

    method parameter is Vector3 class, 
    method should return infromations about object int the nearest point */

rtrace::Collision Plane::distance(const rtrace::Vector3 &point) const {
    rtrace::Collision data;         // construct fills struct with correct fields

    const rtrace::Vector3 P(0, 0, -this->D/this->C);
    const rtrace::Vector3 N = this->getNormal();
    const rtrace::Vector3 V = point - P;

    data.distance = std::abs(V*N);                  // set distance from surface to point (with sign)
    data.point = point;                             // set point, where calculations are
    data.normal = N;                                // set normal in the nearest point
    data.material = this->material;                 // set material of the objects
    data.exist = (data.distance<rtrace::EPSILON);	// set true if point is near enough

    return data;
}
