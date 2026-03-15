#ifndef RTRACE_LIGHT_HPP
#define RTRACE_LIGHT_HPP

#include "rtrace/vector.h"

namespace rtrace {

/*
LightSource class
It is part of the scene. At this moment describes only position of light point.
*/

class Light {
    Vector3 position;

public:
    Light();
    Light(const Vector3 &);
    const Vector3 &getPosition() const;
};

}

#endif
