#include "plane.h"

Plane::Plane(const rtrace::Vector3 &point, const rtrace::Vector3 &n) : point{point} {
	normal = rtrace::normalize(n);
}

rtrace::Color Plane::getColor(double u, double v) const {
	
	constexpr double size = 25;

	if(u<0.) {
		u = -u + size;
	}

	if(v<0.) {
		v = -v + size;
	}

	if((std::fmod(u, 2.*size)<size)==(std::fmod(v, 2.*size)<size)) {
		return rtrace::Color(64, 64, 64);
	}

	return rtrace::Color(0, 0, 0);
}

/*  CollisionData CustomObject::intersect(const Ray &) const

    method parameter is Ray struct, 
    if user objects collide with that ray, should return correct CollisionData struct
    if not, should return not changed CollisionData struct  */

rtrace::Collision Plane::intersect(const rtrace::Ray &ray) const {
    rtrace::Collision data;

	if(normal*ray.direction>-rtrace::EPSILON)
		return data;

	const rtrace::Vector3 dirU = rtrace::normalize(normal^rtrace::Vector3::X);
	const rtrace::Vector3 dirV = rtrace::normalize(normal^dirU);

    const double t = -(((dirU^dirV)*(ray.origin - point))/(ray.direction*(dirU^dirV)));

	const rtrace::Vector3 p = ray.origin + t*ray.direction;
	const double u = (p - point)*dirU;
	const double v = (p - point)*dirV;

	data.point = p;														// set collision point
    data.normal = normal;    											// set surface notrmal vector in collision point
    data.material = rtrace::Material(getColor(u, v), 0, 1, 0, 1, 0.3);	// set material in this point
    data.distance = t; 													// set distance from ray origin to collision point
    data.exist = true;         											// collision occured? set to true

    return data;
}

/*  CollisionData CustomObject::distance(const Vector3 &) const

    method parameter is Vector3 class, 
    method should return infromations about object int the nearest point */

rtrace::Collision Plane::distance(const rtrace::Vector3 &p) const {
	rtrace::Collision data;

	const double distance = std::abs((p - point)*normal);

	if(distance<rtrace::EPSILON)
		return data;

    data.distance = distance;														// set distance from surface to point (with sign)
    data.point = p;																	// set point, where calculations are
    data.normal = normal;															// set normal in the nearest point
    data.material = rtrace::Material(rtrace::Color(64, 64, 64), 0, 1, 0, 1, 0.3);	// set material of the objects
    data.exist = true;																// set true if point is near enough

    return data;
}
