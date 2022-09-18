#pragma once

#include <iostream>
#include <cassert>
#include <string>

#include "matrix.h"

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
		Vector3(const Matrix &);

		double getLength() const;
		Vector3 getNormalized() const;
		Matrix getTransposition() const;

		bool operator!=(const Vector3 &) const;
		Vector3 & operator+=(const Vector3 &);
		Vector3 & operator-=(const Vector3 &);
		Vector3 operator+(const Vector3 &) const;
		Vector3 operator-(const Vector3 &) const;
		Vector3 operator^(const Vector3 &) const;
		double operator*(const Vector3 &) const;
		Matrix operator*(const Matrix &) const;
		Vector3 operator*(const double &) const;
		Vector3 operator/(const double &) const;

		static Vector3 UnitX();
		static Vector3 UnitY();
		static Vector3 UnitZ();
	};

	Vector3 operator*(const double &, const Vector3 &);
	Vector3 operator*(const Matrix &, const Vector3 &);
	Vector3 operator-(const Vector3 &);

	double length(const Vector3 &);
	Vector3 normalize(const Vector3 &);
	Vector3 rotate(const Vector3 &, const Vector3 &, const double &);
	std::ostream & operator<<(std::ostream &, const Vector3 &);

}
