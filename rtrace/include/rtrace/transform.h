#pragma once

#include <cmath>
#include <rtrace/tools.h>
#include <rtrace/ray.h>

namespace rtrace {

	class Transform3 {
	private:
		Matrix33 rotation;
		Vector3 translation;

	public:
		Transform3();

		void translate(const Vector3 &);
		void rotate(const Vector3&, const double&);

		Vector3 convertWordToFrame(const Vector3&) const;
		Vector3 convertFrameToWord(const Vector3&) const;

		Ray convertWordToFrame(const Ray&) const;
		Ray convertFrameToWord(const Ray&) const;

		const Vector3 & getTranslation() const;
		const Matrix33 & getRotation() const;
	};

}
