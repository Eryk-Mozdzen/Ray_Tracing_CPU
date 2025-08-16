#include <rtrace/boundingbox.h>
#include <algorithm>
#include <vector>

namespace rtrace {

	BoundingBox::BoundingBox(Transform3 &transform, Vector3 min, Vector3 max) : 
			transform{transform}, minimum{min}, maximum{max} {

	}

	bool BoundingBox::intersect(const Ray &ray) const {

		const std::vector<Vector3> verts = {
			{minimum.x, minimum.y, minimum.z},
			{maximum.x, minimum.y, minimum.z},
			{minimum.x, maximum.y, minimum.z},
			{maximum.x, maximum.y, minimum.z},
			{minimum.x, minimum.y, maximum.z},
			{maximum.x, minimum.y, maximum.z},
			{minimum.x, maximum.y, maximum.z},
			{maximum.x, maximum.y, maximum.z}
		};

		std::vector<Vector3> rotated;

		std::transform(verts.begin(), verts.end(), std::back_inserter(rotated), [this](const Vector3 &vert) {
			return transform.convertFrameToWord(vert);
		});

		Vector3 min = rotated.front();
		Vector3 max = rotated.front();
		for(const Vector3 &vert : rotated) {
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

}
