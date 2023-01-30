#pragma once

#include <iostream>
#include <array>

namespace rtrace {

	/*  Matrix class
		This is independent file, can be used in other projects.
		Class contain basic operators overload, methods and capabilities 
		to operate with matrices in math or physics.                        */

	class Matrix33 {
	private:
		std::array<double, 9> data;

	public:
		Matrix33();

		Matrix33 operator*(const Matrix33&) const;

		const double & operator()(int, int) const;
		double & operator()(int, int);
	};

	std::ostream& operator<<(std::ostream&, const Matrix33&);

	double determinant(const Matrix33&);
	Matrix33 transposition(const Matrix33&);
}
