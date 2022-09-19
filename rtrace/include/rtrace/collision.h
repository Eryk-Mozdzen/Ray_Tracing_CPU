#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#include <rtrace/material.h>
#include <rtrace/vector.h>
#include <rtrace/tools.h>

namespace rtrace {

	/*  Collision struct
		Struct contains all data requied to calculate reflections and other stuff.
		When method intersect() or distance() returns this struct, it should
		be initialized by user with correct values (like in example).               */

	struct Collision {
		bool exist;             // true if collision occurred
		Vector3 point;          // point of collision
		Vector3 normal;         // normalized normal to surface in specific point
		Material material;      // material of objects where the collision occurred
		double distance;        // distance to the object

		Collision();

		static Collision min(const Collision&, const Collision&);
		static Collision smin(const Collision&, const Collision&, const double&);
	};

}
