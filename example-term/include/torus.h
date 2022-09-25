#pragma once

#include <algorithm>
#include <rtrace/object.h>
#include <rtrace/transform.h>

class Torus : public rtrace::Object {
private:
    double majorRadius, minorRadius;

public:
	rtrace::Transform3 transform;

    Torus(rtrace::Vector3, double, double);

    rtrace::Collision intersect(const rtrace::Ray&) const;
	rtrace::Collision distance(const rtrace::Vector3&) const;
};
