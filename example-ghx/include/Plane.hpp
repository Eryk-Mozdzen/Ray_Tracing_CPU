#pragma once

#include <rtrace/object.h>

/*  User custom Plane class
    It is drawable object looks like infinity plane, flat ground.    */

class Plane : public rtrace::Object {
private:
	rtrace::Vector3 normal;
	rtrace::Vector3 point;
	
	rtrace::Color getColor(double, double) const;
public:
    Plane(rtrace::Vector3, rtrace::Vector3);

    rtrace::Collision intersect(const rtrace::Ray&) const;
    rtrace::Collision distance(const rtrace::Vector3&) const;
};
