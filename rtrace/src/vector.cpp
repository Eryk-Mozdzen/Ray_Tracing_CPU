#include <rtrace/vector.h>
#include <cmath>

namespace rtrace {

	const Vector3 Vector3::X(1, 0, 0);
	const Vector3 Vector3::Y(0, 1, 0);
	const Vector3 Vector3::Z(0, 0, 1);

	Vector3::Vector3() : x{0}, y{0}, z{0} {

	}

	Vector3::Vector3(const double &x, const double &y, const double &z) : x{x}, y{y}, z{z} {

	}

	Vector3 & Vector3::operator+=(const Vector3 &rhs) {
		x +=rhs.x;
		y +=rhs.y;
		z +=rhs.z;

		return *this;
	}

	Vector3 & Vector3::operator-=(const Vector3 &rhs) {
		x -=rhs.x;
		y -=rhs.y;
		z -=rhs.z;

		return *this;
	}

	Vector3 Vector3::operator+(const Vector3 &rhs) const {
		return Vector3(
			x + rhs.x,
			y + rhs.y,
			z + rhs.z
		);
	}

	Vector3 Vector3::operator-(const Vector3 &rhs) const {
		return Vector3(
			x - rhs.x,
			y - rhs.y,
			z - rhs.z
		);
	}

	double Vector3::operator*(const Vector3 &rhs) const {
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	Vector3 Vector3::operator^(const Vector3 &rhs) const {
		return Vector3(
			y*rhs.z - z*rhs.y,
			-(x*rhs.z - z*rhs.x),
			x*rhs.y - y*rhs.x
		);
	}

	Vector3 Vector3::operator*(const double &rhs) const {
		return Vector3(
			x*rhs,
			y*rhs,
			z*rhs
		);
	}

	Vector3 Vector3::operator/(const double &rhs) const {
		return Vector3(
			x/rhs,
			y/rhs,
			z/rhs
		);
	}

	Vector3 operator*(const double &lhs, const rtrace::Vector3 &rhs) {
		return Vector3(
			lhs*rhs.x,
			lhs*rhs.y,
			lhs*rhs.z
		);
	}

	Vector3 operator*(const Matrix33 &matrix, const Vector3 &vec) {
		Vector3 result;

		result.x = matrix(0, 0)*vec.x + matrix(0, 1)*vec.y + matrix(0, 2)*vec.z;
		result.y = matrix(1, 0)*vec.x + matrix(1, 1)*vec.y + matrix(1, 2)*vec.z;
		result.z = matrix(2, 0)*vec.x + matrix(2, 1)*vec.y + matrix(2, 2)*vec.z;

		return result;
	}

	Vector3 operator-(const Vector3 &rhs) {
		return Vector3(
			-rhs.x, 
			-rhs.y, 
			-rhs.z
		);
	}

	std::ostream & operator<<(std::ostream &lhs, const Vector3 &rhs) {
		lhs << "[";
		lhs << rhs.x << ", ";
		lhs << rhs.y << ", ";
		lhs << rhs.z;
		lhs << "]";
		return lhs;
	}

	double length(const Vector3 &vec) {
		return std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	}

	Vector3 normalize(const Vector3 &vec) {
		const double len = length(vec);

		if(len<EPSILON)
			return Vector3();

		return Vector3(
			vec.x/len,
			vec.y/len,
			vec.z/len
		);
	}

}
