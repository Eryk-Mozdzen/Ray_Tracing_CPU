#include "rtrace/Ray.hpp"

namespace rtrace {

Ray::Ray() {
}

Ray::Ray(const Vector3 &origin, const Vector3 &dir) : origin{origin}, direction{dir} {
}

}
