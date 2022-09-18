#include "Matrix.h"
#include <string>
#include <iomanip>

Matrix::Matrix() {}

Matrix::Matrix(const unsigned int &rows, const unsigned int &cols) {
    this->rows = rows;
    this->cols = cols;

    this->values.resize(rows);
    for(unsigned int i=0; i<rows; i++)
        this->values[i].resize(cols, 0);
}

Matrix Matrix::getTransposition() const {
    Matrix result(this->getCols(), this->getRows());

    for(unsigned int i=0; i<result.getRows(); i++)
        for(unsigned int j=0; j<result.getCols(); j++)
            result(i, j) = (*this)(j, i);

    return result;
}

Matrix Matrix::getInverse() const {
    return Matrix::Inverse(*this);
}

Matrix Matrix::Inverse(const Matrix &A) {
    assert(A.getRows()==A.getCols());

    unsigned int n = A.getRows();
    double det = Matrix::Det(A);

	if(std::abs(det)<EPSILON) {
		std::cout << "Inverse matrix does not exit" << std::endl;
		return Matrix::Identity(n);
	}

    if(n==1) {
        Matrix result(1, 1);
        result(0, 0) = 1/A(0, 0);
        return result;
    }

	if(n==3) {
		// https://stackoverflow.com/questions/983999/simple-3x3-matrix-inverse-code-c

		const double invdet = 1./det;

		Matrix minv(3, 3);
		minv(0, 0) = (A(1, 1) * A(2, 2) - A(2, 1) * A(1, 2)) * invdet;
		minv(0, 1) = (A(0, 2) * A(2, 1) - A(0, 1) * A(2, 2)) * invdet;
		minv(0, 2) = (A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1)) * invdet;
		minv(1, 0) = (A(1, 2) * A(2, 0) - A(1, 0) * A(2, 2)) * invdet;
		minv(1, 1) = (A(0, 0) * A(2, 2) - A(0, 2) * A(2, 0)) * invdet;
		minv(1, 2) = (A(1, 0) * A(0, 2) - A(0, 0) * A(1, 2)) * invdet;
		minv(2, 0) = (A(1, 0) * A(2, 1) - A(2, 0) * A(1, 1)) * invdet;
		minv(2, 1) = (A(2, 0) * A(0, 1) - A(0, 0) * A(2, 1)) * invdet;
		minv(2, 2) = (A(0, 0) * A(1, 1) - A(1, 0) * A(0, 1)) * invdet;

		return minv;
	}

	Matrix A_d(n, n);
	for(unsigned int i=0; i<n; i++) {
		for(unsigned int j=0; j<n; j++) {
			Matrix tmp(n-1, n-1);
			unsigned int index = 0, index_tmp = 0;
			while(index<n*n) {

				if(index%n!=j && index/n!=i) {
					tmp(index_tmp%(n-1), index_tmp/(n-1)) = A(index%n, index/n);
					index_tmp++;
				}

				index++;
			}
			A_d(j, i) = Matrix::Det(tmp)*std::pow(-1, i+j);
		}
	}

	return (1/det)*A_d.getTransposition();
}

bool Matrix::operator==(const Matrix &rhs) const {
    assert(this->getRows()==rhs.getRows());
    assert(this->getCols()==rhs.getCols());

    for(unsigned int i=0; i<this->getRows(); i++)
        for(unsigned int j=0; j<this->getCols(); j++)
            if((*this)(i, j)!=rhs(i, j))
                return false;

    return true;
}

bool Matrix::operator!=(const Matrix &rhs) const {
    return !(*this==rhs);
}

Matrix Matrix::operator+(const Matrix &rhs) const {
    assert(this->getRows()==rhs.getRows());
    assert(this->getCols()==rhs.getCols());

    Matrix result(this->getRows(), this->getCols());
    for(unsigned int i=0; i<result.getRows(); i++)
        for(unsigned int j=0; j<result.getCols(); j++)
            result(i, j) = (*this)(i, j) + rhs(i, j);

    return result;
}

Matrix Matrix::operator-(const Matrix &rhs) const {
    return *this + (-rhs);
}

Matrix Matrix::operator*(const Matrix &rhs) const {
    assert(this->getCols()==rhs.getRows());

    Matrix result(this->getRows(), rhs.getCols());
    for(unsigned int i=0; i<result.getRows(); i++) {
        for(unsigned int j=0; j<result.getCols(); j++) {
            for(unsigned int k=0; k<this->cols; k++)
                result(i, j) +=((*this)(i, k) * rhs(k, j));
        }
    }

    return result;
}

Matrix Matrix::operator*(const double &rhs) const {
    Matrix result(this->getRows(), this->getCols());

    for(unsigned int i=0; i<result.getRows(); i++)
        for(unsigned int j=0; j<result.getCols(); j++)
            result(i, j) = (*this)(i, j) * rhs;

    return result;
}

Matrix Matrix::operator/(const double &rhs) const {
    return (*this)*(1/rhs);
}

const double & Matrix::operator()(const unsigned int &row, const unsigned int &col) const {
    assert(row>=0 && row<this->getRows());
    assert(col>=0 && col<this->getCols());

    return this->values[row][col];
}

double & Matrix::operator()(const unsigned int &row, const unsigned int &col) {
    assert(row>=0 && row<this->getRows());
    assert(col>=0 && col<this->getCols());

    return this->values[row][col];
}

const unsigned int & Matrix::getRows() const {
    return this->rows;
}

const unsigned int & Matrix::getCols() const {
    return this->cols;
}

Matrix operator*(const double &lhs, const Matrix &rhs) {
    return rhs*lhs;
}

Matrix operator-(const Matrix &A) {
    Matrix result = A;
    for(unsigned int i=0; i<result.getRows(); i++)
        for(unsigned int j=0; j<result.getCols(); j++)
            result(i, j) = -result(i, j);
    return result;
}

Matrix Matrix::Identity(const unsigned int &n) {
    Matrix identity(n, n);
    for(unsigned int i=0; i<n; i++)
        identity(i, i) = 1;
    return identity;
}

double Matrix::Det(const Matrix &A) {
    assert(A.getRows()==A.getCols());

    const unsigned int n = A.getRows();

    if(n==1)
        return A(0, 0);

    Matrix B = A;

    for(unsigned int i=0; i<n-1; i++) {
        const double top = B(i, i);

        for(unsigned int j=i+1; j<n; j++) {
            const double factor = B(j, i)/top;
            for(unsigned int k=0; k<n; k++) {
                B(j, k) -=factor*B(i, k);
            }
        }
    }

    double det = 1;
    for(unsigned int i=0; i<n; i++)
        det *=B(i, i);

    return det;
}

std::ostream & operator<<(std::ostream &lhs, const Matrix &rhs) {
    lhs.precision(5);
    lhs.setf(std::ios_base::fixed);

    lhs << "[";
    for(unsigned int i=0; i<rhs.getRows(); i++) {
        if(i!=0)
            lhs << " ";
        lhs << "[";
        for(unsigned int j=0; j<rhs.getCols(); j++) {
            lhs << rhs(i, j);
            if(j!=rhs.getCols()-1)
                lhs << ",\t";
        }
        lhs << "]";
        if(i!=rhs.getRows()-1)
            lhs << "," << std::endl;
    }
    lhs << "]";

    //lhs.unsetf(std::ios_base::fixed);

    return lhs;
}

