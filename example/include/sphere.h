#pragma once

#include <rtrace/object.h>
#include <rtrace/transform.h>

/*  User custom Sphere class
    It is drawable object looks like ball. Can have color or texture.    */

class Sphere : public rtrace::Object {
private:
    double radius;
	rtrace::Material material;

public:
	rtrace::Transform3 transform;

    Sphere(const rtrace::Vector3&, const double&, const rtrace::Material&);

    rtrace::Collision intersect(const rtrace::Ray&) const;
    rtrace::Collision distance(const rtrace::Vector3&) const;
};
