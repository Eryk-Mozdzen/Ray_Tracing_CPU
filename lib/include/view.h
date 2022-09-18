#pragma once

#include <vector>
#include <cmath>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "vector.h"
#include "matrix.h"
#include "transform.h"

namespace rtrace {

	/*  View class
		View describes point in space where eye is
		and direction where eye is pointing.
		To render a frame you need to specify specific view.     */

	class View {
	private:
		double distance;
		Transform3 transform;
	public:
		View();
		View(const Vector3&, const double&);

		Vector3 getDirectionX() const;
		Vector3 getDirectionY() const;
		Vector3 getDirectionZ() const;
		Vector3 getPosition() const;
		const Transform3 & getTransform() const;
		double getDistanceFromProjectionPlane() const;

		void setDistanceFromProjectionPlane(const double&);
		void translate(const Vector3&);
		void rotate(const Vector3&, const double&);
	};

}
