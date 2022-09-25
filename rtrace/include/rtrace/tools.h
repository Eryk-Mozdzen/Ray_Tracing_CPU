#pragma once

#include <cmath>
#include <vector>
#include <complex>
#include <rtrace/vector.h>
#include <rtrace/matrix.h>

namespace rtrace {

	constexpr double EPSILON = 0.001;

	std::vector<double> findZerosPoly2(double, double, double);
	std::vector<double> findZerosPoly4(std::complex<double>, std::complex<double>, std::complex<double>, std::complex<double>, std::complex<double>);

	Vector3 operator*(const Matrix33&, const Vector3&);
	Vector3 solveLinearSystem(const Matrix33&, const Vector3&);
}
