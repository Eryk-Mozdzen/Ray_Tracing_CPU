#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <rtrace/Object.hpp>
#include <rtrace/Transform.hpp>

/*
User custom Sphere class
It is drawable object looks like ball. Can have color or texture.
*/

class Sphere : public rtrace::Object {
    double radius;
    rtrace::Material material;

public:
    rtrace::Transform3 transform;

    Sphere(rtrace::Vector3, double, rtrace::Material);

    rtrace::Collision intersect(const rtrace::Ray &) const;
    rtrace::Collision distance(const rtrace::Vector3 &) const;
};

#endif
