#include <rtrace/ray.h>

rtrace::Ray::Ray() {}

rtrace::Ray::Ray(const rtrace::Vector3 &origin, const rtrace::Vector3 &direction) : origin{origin}, direction{rtrace::normalize(direction)} {

}
