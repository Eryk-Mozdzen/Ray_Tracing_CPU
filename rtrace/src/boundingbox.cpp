#include <rtrace/boundingbox.h>

rtrace::BoundingBox::BoundingBox(rtrace::Transform3 &transform, rtrace::Vector3 min, rtrace::Vector3 max) : 
		transform{transform}, minimum{min}, maximum{max} {

}

bool rtrace::BoundingBox::intersect(const rtrace::Ray &ray) const {

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

	// https://gdbooks.gitbooks.io/3dcollisions/content/Chapter3/raycast_aabb.html
	const double t1 = (min.x - ray.origin.x)/ray.direction.x;
	const double t2 = (max.x - ray.origin.x)/ray.direction.x;
	const double t3 = (min.y - ray.origin.y)/ray.direction.y;
	const double t4 = (max.y - ray.origin.y)/ray.direction.y;
	const double t5 = (min.z - ray.origin.z)/ray.direction.z;
	const double t6 = (max.z - ray.origin.z)/ray.direction.z;

	const double tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    const double tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	return (tmax>0 && tmin<tmax);
}
