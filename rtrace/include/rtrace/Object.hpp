#ifndef RTRACE_OBJECT_HPP
#define RTRACE_OBJECT_HPP

#include "rtrace/Collision.hpp"
#include "rtrace/Ray.hpp"
#include "rtrace/Vector.hpp"

namespace rtrace {

/*  Object class
    Class can be drawn on the scene.
    When user want to inheriting them form this class
    and overwrite one from distance() or intersect() methods (like in example).
    When custom object has only one of this methods,
    then will be appear only in corresponding mode.  */

class Object {
public:
    virtual ~Object() {};
    virtual Collision intersect(const Ray &) const = 0;    // for ray tracing
    virtual Collision distance(const Vector3 &) const = 0; // for sphere tracing
};

}

#endif
