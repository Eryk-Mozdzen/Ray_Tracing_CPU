#pragma once

#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <vector>

namespace rtrace {

	constexpr double EPSILON = 0.001;

	/*  Matrix class
		This is independent file, can be used in other projects.
		Class contain basic operators overload, methods and capabilities 
		to operate with matrices in math or physics.                        */

	class Matrix {
	private:
		std::vector<std::vector<double>> values;
		unsigned int rows, cols;
	public:
		Matrix();
		Matrix(const unsigned int&, const unsigned int&);

		Matrix getTransposition() const;
		Matrix getInverse() const;

		bool operator==(const Matrix&) const;
		bool operator!=(const Matrix&) const;
		Matrix operator+(const Matrix&) const;
		Matrix operator-(const Matrix&) const;
		Matrix operator*(const Matrix&) const;
		Matrix operator*(const double&) const;
		Matrix operator/(const double&) const;

		const double & operator()(const unsigned int&, const unsigned int&) const;
		double & operator()(const unsigned int&, const unsigned int&);
		const unsigned int & getRows() const;
		const unsigned int & getCols() const;

		static Matrix Inverse(const Matrix&);
		static Matrix Identity(const unsigned int&);
		static double Det(const Matrix&);
	};

	Matrix operator*(const double&, const Matrix&);
	Matrix operator-(const Matrix&);

	std::ostream & operator<<(std::ostream&, const Matrix&);

}
