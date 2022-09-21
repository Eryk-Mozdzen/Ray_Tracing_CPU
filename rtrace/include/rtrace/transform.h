#pragma once

#include <cmath>
#include <rtrace/tools.h>

namespace rtrace {

	/*  Transform3 class
		Class represents 3-dimensional transform matrix (4x4) to
		describes translation and rotation of objects in scene.     */

	class Transform3 : public Matrix<4, 4> {
	public:
		Transform3();
		Transform3& operator=(const Matrix<4, 4>&);

		void translate(const Vector3 &d);
		void rotate(const Vector3&, const double&);

		Vector3 getRelativeToTransform(const Vector3&) const;
		Vector3 getRelativeToReferenceFrame(const Vector3&) const;

		Vector3 getTranslation() const;
		Matrix<3, 3> getRotation() const;
	};

}
