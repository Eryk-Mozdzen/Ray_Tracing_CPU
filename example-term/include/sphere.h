#pragma once

#include <algorithm>
#include <rtrace/object.h>
#include <rtrace/transform.h>

class Sphere : public rtrace::Object {
private:
    double radius;

public:
	rtrace::Transform3 transform;

    Sphere(rtrace::Vector3, double);

    rtrace::Collision intersect(const rtrace::Ray&) const;
	rtrace::Collision distance(const rtrace::Vector3&) const;
};
