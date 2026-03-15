#ifndef RTRACE_RAY_HPP
#define RTRACE_RAY_HPP

#include "rtrace/Vector.hpp"

namespace rtrace {

/*
Ray struct
Struct contains origin point and normalized direction vector
*/

struct Ray {
    Vector3 origin;
    Vector3 direction;

    Ray();
    Ray(const Vector3 &, const Vector3 &);
};

}

#endif
