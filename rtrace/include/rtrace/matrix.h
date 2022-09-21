#pragma once

#include <iostream>
#include <iomanip>
#include <array>
#include <algorithm>

namespace rtrace {

	/*  Matrix class
		This is independent file, can be used in other projects.
		Class contain basic operators overload, methods and capabilities 
		to operate with matrices in math or physics.                        */

	template<int M, int N>
	class Matrix {
	private:
		std::array<double, N*M> data;
	public:
		Matrix();

		Matrix<M, N> operator+(const Matrix<M, N>&) const;
		Matrix<M, N> operator-(const Matrix<M, N>&) const;
		Matrix<M, N> operator*(const double&) const;
		Matrix<M, N> operator/(const double&) const;

		const double & operator()(const int&, const int&) const;
		double & operator()(const int&, const int&);
	};

	template<int M, int N, int K>
	Matrix<M, K> operator*(const Matrix<M, N>&, const Matrix<N, K>&);
	template<int M, int N>
	Matrix<M, N> operator*(const double&, const Matrix<M, N>&);
	template<int M, int N>
	Matrix<M, N> operator-(const Matrix<M, N>&);
	template<int M, int N>
	std::ostream& operator<<(std::ostream&, const Matrix<M, N>&);

	template<int M>
	double determinant(const Matrix<M, M>&);
}

template<int M, int N>
rtrace::Matrix<M, N>::Matrix() : data{0} {

}

template<int M, int N>
rtrace::Matrix<M, N> rtrace::Matrix<M, N>::operator+(const rtrace::Matrix<M, N> &other) const {
	rtrace::Matrix<M, N> result;

	for(int i=0; i<N*M; i++) {
		result.data[i] = data[i] + other.data[i];
	}

    return result;
}

template<int M, int N>
rtrace::Matrix<M, N> rtrace::Matrix<M, N>::operator-(const rtrace::Matrix<M, N> &other) const {
	rtrace::Matrix<M, N> result;

	for(int i=0; i<N*M; i++) {
		result.data[i] = data[i] - other.data[i];
	}

    return result;
}

template<int M, int N, int K>
rtrace::Matrix<M, K> rtrace::operator*(const rtrace::Matrix<M, N> &matrix, const rtrace::Matrix<N, K> &other) {
	rtrace::Matrix<M, K> result;

    for(int i=0; i<M; i++) {
        for(int j=0; j<N; j++) {
            for(int k=0; k<K; k++) {
				result(i, k) +=(matrix(i, j)*other(j, k));
			}
        }
    }

    return result;
}

template<int M, int N>
rtrace::Matrix<M, N> rtrace::Matrix<M, N>::operator*(const double &number) const {
	rtrace::Matrix<M, N> result;

	for(int i=0; i<N*M; i++) {
		result.data[i] = data[i]*number;
	}

    return result;
}

template<int M, int N>
rtrace::Matrix<M, N> rtrace::Matrix<M, N>::operator/(const double &number) const {
	rtrace::Matrix<N, M> result;

	for(int i=0; i<N*M; i++) {
		result.data[i] = data[i]/number;
	}

    return result;
}

template<int M, int N>
const double & rtrace::Matrix<M, N>::operator()(const int &row, const int &col) const {
    return data[row*N + col];
}

template<int M, int N>
double & rtrace::Matrix<M, N>::operator()(const int &row, const int &col) {
    return data[row*N + col];
}

template<int M, int N>
rtrace::Matrix<M, N> rtrace::operator*(const double &number, const rtrace::Matrix<M, N> &matrix) {
	rtrace::Matrix<M, N> result;

	for(int i=0; i<M; i++) {
		for(int j=0; j<N; j++) {
			result(i, j) = number*matrix(i, j);
		}
	}

    return result;
}

template<int M, int N>
rtrace::Matrix<M, N> rtrace::operator-(const rtrace::Matrix<M, N> &matrix) {
	rtrace::Matrix<M, N> result;

	for(int i=0; i<M; i++) {
		for(int j=0; j<N; j++) {
			result(i, j) = -matrix(i, j);
		}
	}

    return result;
}

template<int M, int N>
std::ostream& operator<<(std::ostream &stream, const rtrace::Matrix<M, N> &matrix) {
	stream.precision(5);
    stream.setf(std::ios_base::fixed);

    stream << "[";
    for(int i=0; i<M; i++) {
        if(i!=0)
            stream << " ";
        stream << "[";
        for(int j=0; j<N; j++) {
            stream << matrix(i, j);
            if(j!=N-1)
                stream << ",\t";
        }
        stream << "]";
        if(i!=M-1)
            stream << "," << std::endl;
    }
    stream << "]";

    return stream;
}

template<int M>
double rtrace::determinant(const Matrix<M, M> &m) {
	if(M==1) {
		return m(0, 0);
	}

	if(M==2) {
		return m(0, 0)*m(1, 1) - m(0, 1)*m(1, 0);
	}

	if(M==3) {
		return 
			m(0, 0)*m(1, 1)*m(2, 2) + 
			m(0, 1)*m(1, 2)*m(2, 0) + 
			m(0, 2)*m(1, 0)*m(2, 1) - 
			m(0, 2)*m(1, 1)*m(2, 0) - 
			m(0, 1)*m(1, 0)*m(2, 2) - 
			m(0, 0)*m(1, 2)*m(2, 1);
	}

    rtrace::Matrix<M, M> B = m;

    for(int i=0; i<M-1; i++) {
        const double top = B(i, i);

        for(int j=i+1; j<M; j++) {
            const double factor = B(j, i)/top;

            for(int k=0; k<M; k++) {
                B(j, k) -=factor*B(i, k);
            }
        }
    }

    double det = 1;
    for(int i=0; i<M; i++)
        det *=B(i, i);

    return det;
}
