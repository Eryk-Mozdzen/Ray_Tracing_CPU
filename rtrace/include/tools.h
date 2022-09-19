#pragma once

#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include <complex>

#include "vector.h"
#include "matrix.h"

#include <SFML/Graphics.hpp>

namespace rtrace {

	/*  Collection of simple math solvers to make cleaner code.
		Not every one is optimalized, but every on works in some way.  */

	double solveLinearEquation(const double&, const double&);
	std::pair<double, double> solveQuadraticEquation(const double&, const double&, const double&);
	std::vector<double> solveQuarticEquation(double, double, double, double, double);

	double calculatePolynomial(const std::vector<double>&, const double&);

	Matrix solveLinearSystemCramersRule(const Matrix&, const Matrix&);
	Matrix solveLinearSystemJacobiMethod(const Matrix&, const Matrix&);

	/*  handy funstions for SFML native Color class     */

	std::ostream & operator<<(std::ostream&, const sf::Color&);
	sf::Color operator*(const double&, const sf::Color&);
	sf::Color color_interpolation(const sf::Color&, const sf::Color&, const double&);

}
