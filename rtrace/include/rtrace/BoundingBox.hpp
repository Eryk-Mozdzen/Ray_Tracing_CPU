#ifndef RTRACE_BOUNDING_BOX_HPP
#define RTRACE_BOUNDING_BOX_HPP

#include "rtrace/ray.h"
#include "rtrace/transform.h"

namespace rtrace {

class BoundingBox {
    Transform3 &transform;
    Vector3 minimum;
    Vector3 maximum;

public:
    BoundingBox(Transform3 &, Vector3, Vector3);

    bool intersect(const Ray &) const;
};

}

#endif
