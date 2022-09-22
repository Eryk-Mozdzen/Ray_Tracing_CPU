#include <rtrace/boundingbox.h>

rtrace::BoundingBox::BoundingBox(rtrace::Transform3 &transform, rtrace::Vector3 min, rtrace::Vector3 max) : 
		transform{transform}, minimum{min}, maximum{max} {

}

bool rtrace::BoundingBox::intersect(const rtrace::Ray &r) const {

	const std::vector<rtrace::Vector3> verts = {
		transform.getRelativeToReferenceFrame({minimum.x, minimum.y, minimum.z}),
		transform.getRelativeToReferenceFrame({maximum.x, minimum.y, minimum.z}),
		transform.getRelativeToReferenceFrame({minimum.x, maximum.y, minimum.z}),
		transform.getRelativeToReferenceFrame({maximum.x, maximum.y, minimum.z}),
		transform.getRelativeToReferenceFrame({minimum.x, minimum.y, maximum.z}),
		transform.getRelativeToReferenceFrame({maximum.x, minimum.y, maximum.z}),
		transform.getRelativeToReferenceFrame({minimum.x, maximum.y, maximum.z}),
		transform.getRelativeToReferenceFrame({maximum.x, maximum.y, maximum.z})
	};

	rtrace::Vector3 min = verts.front();
	rtrace::Vector3 max = verts.front();
	for(const rtrace::Vector3 &vert : verts) {
		min.x = std::min(min.x, vert.x);
		min.y = std::min(min.y, vert.y);
		min.z = std::min(min.z, vert.z);

		max.x = std::max(max.x, vert.x);
		max.y = std::max(max.y, vert.y);
		max.z = std::max(max.z, vert.z);
	}

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
