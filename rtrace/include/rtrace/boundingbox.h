#pragma once

#include <rtrace/ray.h>
#include <rtrace/transform.h>

namespace rtrace {

	class BoundingBox {
	private:
		Transform3 &transform;
		Vector3 min;
		Vector3 max;

	public:
		BoundingBox(Transform3 &, Vector3, Vector3);

		bool intersect(const Ray &) const;
	};

}
