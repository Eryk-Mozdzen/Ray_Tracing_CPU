#pragma once

#include <rtrace/object.h>

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
