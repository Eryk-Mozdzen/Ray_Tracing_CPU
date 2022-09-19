#pragma once

#include "material.h"
#include "transform.h"
#include "collision.h"
#include "ray.h"

namespace rtrace {

	/*  Object class
		Class can be drawn on the scene.
		Whene user want to inheriting them form this class
		and overrite one from distance() or intersect() methods (like in example).
		When custom object has only one of this methods,
		then will be appear only in coresponding mode.                                  */

	class Object {
	protected:
		Material material;
		Transform3 transform;
	public:
		virtual ~Object() {};
		virtual Collision intersect(const Ray&) const = 0;     // for ray tracing
		virtual Collision distance(const Vector3&) const = 0;  // for sphere tracing

		const Transform3 & getTransform() const;
		const Material & getMaterial() const;
		void setTransform(const Transform3&);
		void setMaterial(const Material&);
	};

}
