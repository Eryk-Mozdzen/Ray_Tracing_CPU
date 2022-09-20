#include "sphere.h"

Sphere::Sphere(rtrace::Vector3 center, double radius, rtrace::Material material) : radius{radius}, material{material} {
	transform.translate(center);
}

/*  Collision CustomObject::intersect(const Ray &) const

    method parameter is Ray struct, 
    if user objects collide with that ray, should return correct CollisionData struct
    if not, should return not changed CollisionData struct  */

rtrace::Collision Sphere::intersect(const rtrace::Ray &ray) const {
    const rtrace::Vector3 center = transform.getTranslation();

    const double a = 1;
    const double b = 2*(ray.direction*(ray.origin - center));
    const double c = rtrace::length(ray.origin - center)*rtrace::length(ray.origin - center) - radius*radius;

    std::vector<double> tSolutions = rtrace::findZerosPoly2(a, b, c);

    if(tSolutions.size()==0) {
		return rtrace::Collision();
	}

	std::remove_if(tSolutions.begin(), tSolutions.end(), [] (const double &solution) {
		return solution<rtrace::EPSILON;
	});

	if(tSolutions.size()==0) {
		return rtrace::Collision();
	}

	const double t = *std::min_element(tSolutions.begin(), tSolutions.end());
	
	rtrace::Collision collision;

    collision.point = ray.origin + t*ray.direction;
    collision.normal = rtrace::normalize(collision.point - center);
    collision.material = material;
    collision.distance = t;
    collision.exist = true;

    return collision;
}

/*  Collision CustomObject::distance(const Vector3 &) const

    method parameter is Vector3 class, 
    method should return infromations about object int the nearest point */

rtrace::Collision Sphere::distance(const rtrace::Vector3 &point) const {
    
    const rtrace::Vector3 center = transform.getTranslation();
	
	rtrace::Collision collision;

    collision.distance = rtrace::length(point - center) - radius;
    collision.point = point;
    collision.normal = rtrace::normalize(point - center);
    collision.material = material;
    collision.exist = (collision.distance<rtrace::EPSILON);
    
    return collision;
}

