#ifndef RTRACE_OBJECT_HPP
#define RTRACE_OBJECT_HPP

#include "rtrace/collision.h"
#include "rtrace/ray.h"
#include "rtrace/vector.h"

namespace rtrace {

/*
Object class
Class can be drawn on the scene.
Whene user want to inheriting them form this class
and overrite one from distance() or intersect() methods (like in example).
When custom object has only one of this methods,
then will be appear only in coresponding mode.
*/

class Object {
public:
    virtual ~Object() {};
    virtual Collision intersect(const Ray &) const = 0;    // for ray tracing
    virtual Collision distance(const Vector3 &) const = 0; // for sphere tracing
};

}

#endif
