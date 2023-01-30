#include <rtrace/transform.h>
#include <cmath>

namespace rtrace {

	Transform3::Transform3() {
		rotation(0, 0) = 1;
		rotation(1, 1) = 1;
		rotation(2, 2) = 1;
	}

	void Transform3::translate(const Vector3 &vec) {
		translation +=rotation*vec;
	}

	void Transform3::rotate(const Vector3 &axis, const double &theta) {
		const Vector3 u = normalize(axis);
		const double s = std::sin(theta);
		const double c = std::cos(theta);

		Matrix33 rot;

		rot(0, 0) = c + u.x*u.x*(1 - c);
		rot(0, 1) = u.x*u.y*(1 - c) - u.z*s;
		rot(0, 2) = u.x*u.z*(1 - c) + u.y*s;

		rot(1, 0) = u.y*u.z*(1 - c) + u.z*s;
		rot(1, 1) = c + u.y*u.y*(1 - c);
		rot(1, 2) = u.y*u.z*(1 - c) - u.x*s;

		rot(2, 0) = u.z*u.x*(1 - c) - u.y*s;
		rot(2, 1) = u.z*u.y*(1 - c) + u.x*s;
		rot(2, 2) = c + u.z*u.z*(1 - c);

		rotation = rotation*rot;
	}

	Vector3 Transform3::convertWordToFrame(const Vector3 &vec) const {
		return transposition(rotation)*(vec - translation);
	}

	Vector3 Transform3::convertFrameToWord(const Vector3 &vec) const {
		return rotation*vec + translation;
	}

	Ray Transform3::convertWordToFrame(const Ray &ray) const {
		const Matrix33 inverse = transposition(rotation);

		return Ray(inverse*(ray.origin - translation), inverse*ray.direction);
	}

	Ray Transform3::convertFrameToWord(const Ray &ray) const {
		return Ray(rotation*ray.origin + translation, rotation*ray.direction);
	}

	const Vector3 & Transform3::getTranslation() const {
		return translation;
	}

	const Matrix33 & Transform3::getRotation() const {
		return rotation;
	}

}
