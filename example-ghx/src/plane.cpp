#include "plane.h"
#include <cmath>

Plane::Plane(rtrace::Vector3 point, rtrace::Vector3 n) : point{point} {
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

rtrace::Collision Plane::intersect(const rtrace::Ray &ray) const {

	if(normal*ray.direction>-rtrace::EPSILON)
		return rtrace::Collision();

	const rtrace::Vector3 dirU = rtrace::normalize(normal^rtrace::Vector3::X);
	const rtrace::Vector3 dirV = rtrace::normalize(normal^dirU);

    const double t = -(((dirU^dirV)*(ray.origin - point))/(ray.direction*(dirU^dirV)));

	const rtrace::Vector3 p = ray.origin + t*ray.direction;
	const double u = (p - point)*dirU;
	const double v = (p - point)*dirV;
	
	rtrace::Collision collision;

	collision.point = p;
    collision.normal = normal;
    collision.material = rtrace::Material(getColor(u, v), 0, 1, 0, 1, 0.3);
    collision.distance = t;
    collision.exist = true;

    return collision;
}

rtrace::Collision Plane::distance(const rtrace::Vector3 &p) const {
	rtrace::Collision collision;

    collision.distance = std::abs((p - point)*normal);
    collision.point = p;
    collision.normal = normal;
    collision.material = rtrace::Material(rtrace::Color(64, 64, 64), 0, 1, 0, 1, 0.3);
    collision.exist = (collision.distance<rtrace::EPSILON);

    return collision;
}
