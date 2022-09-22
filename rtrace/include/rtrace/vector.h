#pragma once

#include <iostream>
#include <cmath>

namespace rtrace {

	/*  Vector class (3-dimensions only)
		This is not independent file, it requires Matrix class to work.
		Class contain basic operators overload, methods and capabilities 
		to operate with 3-dimensional vectors and matrices in math or physics.  */

	class Vector3 {
	public:
		double x, y, z;

		Vector3();
		Vector3(const double &, const double &, const double &);

		Vector3 & operator+=(const Vector3 &);
		Vector3 & operator-=(const Vector3 &);
		Vector3 operator+(const Vector3 &) const;
		Vector3 operator-(const Vector3 &) const;
		Vector3 operator^(const Vector3 &) const;
		double operator*(const Vector3 &) const;
		Vector3 operator*(const double &) const;
		Vector3 operator/(const double &) const;

		static const Vector3 X;
		static const Vector3 Y;
		static const Vector3 Z;
	};

	Vector3 operator*(const double &, const Vector3 &);
	Vector3 operator-(const Vector3 &);
	std::ostream & operator<<(std::ostream &, const Vector3 &);

	double length(const Vector3 &);
	Vector3 normalize(const Vector3 &);

}
