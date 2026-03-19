#ifndef TORUS_HPP
#define TORUS_HPP

#include <rtrace/Object.hpp>
#include <rtrace/Transform.hpp>

class Torus : public rtrace::Object {
    double majorRadius, minorRadius;

public:
    rtrace::Transform3 transform;

    Torus(rtrace::Vector3, double, double);

    rtrace::Collision intersect(const rtrace::Ray &) const;
    rtrace::Collision distance(const rtrace::Vector3 &) const;
};

#endif
