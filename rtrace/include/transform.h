#pragma once

#include <string>
#include <vector>
#include <cmath>

#include "vector.h"
#include "matrix.h"
#include "tools.h"

namespace rtrace {

	/*  Transform3 class
		Class represents 3-dimensional transform matrix (4x4) to
		describes translation and rotation of objects in scene.     */

	class Transform3 : public Matrix {
	public:
		Transform3();
		Transform3 & operator=(const Matrix &);

		void translate(const Vector3 &d);
		void rotate(const Vector3&, const double&);

		Vector3 getRelativeToTransform(const Vector3&) const;
		Vector3 getRelativeToReferenceFrame(const Vector3&) const;

		Vector3 getTranslation() const;
		Matrix getRotation() const;
	};

}
