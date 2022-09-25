#pragma once

#include <cmath>
#include <rtrace/tools.h>

namespace rtrace {

	/*  Transform3 class
		Class represents 3-dimensional transform matrix (4x4) to
		describes translation and rotation of objects in scene.     */

	class Transform3 {
	private:
		Matrix33 rotation;
		Vector3 translation;

	public:
		Transform3();

		void translate(const Vector3 &);
		void rotate(const Vector3&, const double&);

		Vector3 getRelativeToTransform(const Vector3&) const;
		Vector3 getRelativeToReferenceFrame(const Vector3&) const;

		const Vector3 & getTranslation() const;
		const Matrix33 & getRotation() const;
	};

}
