#ifndef RAY_TRACING_MATH_H
#define RAY_TRACING_MATH_H

#include <iostream>
#include <cassert>
#include <cmath>
#include <utility>
#include <vector>
#include <complex>

#include "Vector.h"
#include "Matrix.h"

/*  Collection of simple math solvers to make cleaner code.
    Not every one is optimalized, but every on works in some way.  */

const double EPSILON = 1E-2;                    // very important constant!!! all floating point comparison should be perfomed with this!!!
//const double M_PI = 3.14159265358979323846;

double solveLinearEquation(const double&, const double&);
std::pair<double, double> solveQuadraticEquation(const double&, const double&, const double&);
std::vector<double> solveQuarticEquation(const double&, const double&, const double&, const double&, const double&);

double calculatePolynomial(const std::vector<double>&, const double&);

Matrix solveLinearSystemCramersRule(const Matrix&, const Matrix&);
Matrix solveLinearSystemJacobiMethod(const Matrix&, const Matrix&);

#endif
