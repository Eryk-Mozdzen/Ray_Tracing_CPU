#include "ray.h"

rtrace::Ray::Ray() {}

rtrace::Ray::Ray(const rtrace::Vector3 &origin, const rtrace::Vector3 &direction) {
    this->origin = origin;
    this->direction = direction;
}
