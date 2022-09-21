#pragma once

#include <cmath>
#include <vector>
#include <complex>
#include <rtrace/vector.h>
#include <rtrace/matrix.h>

template class rtrace::Matrix<3, 3>;

namespace rtrace {

	constexpr double EPSILON = 0.001;

	/*  Collection of simple math solvers to make cleaner code.		*/

	Matrix<1, 3> transposition(const Vector3 &);

	double findZeroPoly1(double, double);
	std::vector<double> findZerosPoly2(double, double, double);
	std::vector<double> findZerosPoly4(std::complex<double>, std::complex<double>, std::complex<double>, std::complex<double>, std::complex<double>);

	Vector3 operator*(const Matrix<3, 3> &, const Vector3 &);
	Matrix<3, 3> operator*(const Vector3 &, const Matrix<1, 3> &);
	Vector3 toVec(const Matrix<3, 1> &);
	Matrix<3, 1> toMat(const Vector3 &);

	template<int M>
	Matrix<M, 1> solveLinearSystem(const Matrix<M, M>&, const Matrix<M, 1>&);

}

template<int M>
rtrace::Matrix<M, 1> rtrace::solveLinearSystem(const rtrace::Matrix<M, M> &A, const rtrace::Matrix<M, 1> &b) {
    rtrace::Matrix<M, 1> x;

    const double W = rtrace::determinant(A);

    if(std::abs(W)<EPSILON)
        return x;

    rtrace::Matrix<M, M> A_i;

    for(int i=0; i<M; i++) {
        A_i = A;

        for(int j=0; j<M; j++)
            A_i(j, i) = b(j, 0);

        x(i, 0) = rtrace::determinant(A_i)/W;
    }

    return x;
}

