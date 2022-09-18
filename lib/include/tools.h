#pragma once

#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include <complex>

#include "vector.h"
#include "matrix.h"

/*  Collection of simple math solvers to make cleaner code.
    Not every one is optimalized, but every on works in some way.  */

double solveLinearEquation(const double&, const double&);
std::pair<double, double> solveQuadraticEquation(const double&, const double&, const double&);
std::vector<double> solveQuarticEquation(double, double, double, double, double);

double calculatePolynomial(const std::vector<double>&, const double&);

Matrix solveLinearSystemCramersRule(const Matrix&, const Matrix&);
Matrix solveLinearSystemJacobiMethod(const Matrix&, const Matrix&);
