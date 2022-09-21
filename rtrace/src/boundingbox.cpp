#include <rtrace/boundingbox.h>

rtrace::BoundingBox::BoundingBox(rtrace::Transform3 &transform, rtrace::Vector3 min, rtrace::Vector3 max) : 
		transform{transform}, min{min}, max{max} {

}

bool rtrace::BoundingBox::intersect(const rtrace::Ray &ray) const {

  	const rtrace::Ray r(transform.getRotation() *
		(ray.origin - transform.getTranslation()),
		transform.getRotation() * ray.direction
	);

	// https://www.researchgate.net/publication/220494140_An_Efficient_and_Robust_Ray-Box_Intersection_Algorithm

	double tmin, tmax, tymin, tymax, tzmin, tzmax;

	if(r.direction.x>=0) {
		tmin = (min.x - r.origin.x) / r.direction.x;
		tmax = (max.x - r.origin.x) / r.direction.x;
	} else {
		tmin = (max.x - r.origin.x) / r.direction.x;
		tmax = (min.x - r.origin.x) / r.direction.x;
	}

	if(r.direction.y>=0) {
		tymin = (min.y - r.origin.y) / r.direction.y;
		tymax = (max.y - r.origin.y) / r.direction.y;
	} else {
		tymin = (max.y - r.origin.y) / r.direction.y;
		tymax = (min.y - r.origin.y) / r.direction.y;
	}

	if((tmin>tymax) || (tymin>tmax))
		return false;

	if(tymin>tmin) 
		tmin = tymin;

	if(tymax<tmax)
		tmax = tymax;

	if(r.direction.z>=0) {
		tzmin = (min.z - r.origin.z) / r.direction.z;
		tzmax = (max.z - r.origin.z) / r.direction.z;
	} else {
		tzmin = (max.z - r.origin.z) / r.direction.z;
		tzmax = (min.z - r.origin.z) / r.direction.z;
	}

	if((tmin>tzmax) || (tzmin>tmax))
		return false;

	if(tzmin>tmin)
		tmin = tzmin;

	if(tzmax<tmax)
		tmax = tzmax;

	return (tmax>rtrace::EPSILON);
}
