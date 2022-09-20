#pragma once

#include <cmath>
#include <vector>
#include <complex>

namespace rtrace {

	constexpr double EPSILON = 0.001;

	/*  Collection of simple math solvers to make cleaner code.
		Not every one is optimalized, but every on works in some way.  */

	double findZeroPoly1(double, double);
	std::vector<double> findZerosPoly2(double, double, double);
	std::vector<double> findZerosPoly4(std::complex<double>, std::complex<double>, std::complex<double>, std::complex<double>, std::complex<double>);

}
