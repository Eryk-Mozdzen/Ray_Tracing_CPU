#include "sphere.h"

Sphere::Sphere(rtrace::Vector3 center, double radius) : radius{radius} {
	transform.translate(center);
}

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
    collision.material = rtrace::Color::red;
    collision.distance = t;
    collision.exist = true;

    return collision;
}

rtrace::Collision Sphere::distance(const rtrace::Vector3 &point) const {
	return rtrace::Collision();
}
