#pragma once

#include "object.h"

/*  User custom Sphere class
    It is drawable object looks like ball. Can have color or texture.    */

class Sphere : public rtrace::Object {
private:
    double radius;

public:
    Sphere(const rtrace::Vector3&, const double&, const rtrace::Material&);

    rtrace::Collision intersect(const rtrace::Ray&) const;
    rtrace::Collision distance(const rtrace::Vector3&) const;
};
