#include "Matrix.h"
#include <string>
#include <iomanip>

Matrix::Matrix() {}

Matrix::Matrix(const int &rows, const int &cols) {
    this->rows = rows;
    this->cols = cols;

    this->values.resize(rows);
    for(int i=0; i<rows; i++)
        this->values[i].resize(cols, 0);
}

Matrix Matrix::transposition() const {
    Matrix result(this->cols, this->rows);

    for(int i=0; i<result.rows; i++)
        for(int j=0; j<result.cols; j++)
            result(i, j) = (*this)(j, i);

    return result;
}

Matrix Matrix::inverse() const {
    return Matrix::Inverse(*this);
}

Matrix Matrix::Inverse(const Matrix &input) {
    if(input.getRows()!=input.getCols())
        throw std::string("Try to calculate inverse from non square matrix");

    int n = input.getRows();
    double det = Matrix::Det(input);

    if(n==1) {
        Matrix result(1, 1);
        result(0, 0) = 1/input(0, 0);
        return result;
    } else {
        Matrix A_d(n, n);
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                Matrix tmp(n-1, n-1);
                int index = 0, index_tmp = 0;
                while(index<n*n) {

                    if(index%n!=j && index/n!=i) {
                        tmp(index_tmp%(n-1), index_tmp/(n-1)) = input(index%n, index/n);
                        index_tmp++;
                    }

                    index++;
                }
                A_d(j, i) = Matrix::Det(tmp)*std::pow(-1, i+j);
            }
        }
        return (1/det)*A_d.transposition();
    }
}

Matrix Matrix::operator+(const Matrix &rhs) const {
    if(this->rows!=rhs.rows) throw std::string("Rows not equal");
    if(this->cols!=rhs.cols) throw std::string("Cols not equal");

    Matrix result(this->rows, this->cols);
    for(int i=0; i<result.rows; i++)
        for(int j=0; j<result.cols; j++)
            result.values[i][j] = this->values[i][j] + rhs.values[i][j];

    return result;
}

Matrix Matrix::operator-(const Matrix &rhs) const {
    if(this->rows!=rhs.rows) throw std::string("Rows not equal");
    if(this->cols!=rhs.cols) throw std::string("Cols not equal");

    Matrix result(this->rows, this->cols);
    for(int i=0; i<result.rows; i++)
        for(int j=0; j<result.cols; j++)
            result.values[i][j] = this->values[i][j] - rhs.values[i][j];

    return result;
}

Matrix Matrix::operator*(const Matrix &rhs) const {
    if(this->cols!=rhs.rows) throw std::string("Incompatible sizes");

    Matrix result(this->rows, rhs.cols);
    for(int i=0; i<result.rows; i++) {
        for(int j=0; j<result.cols; j++) {
            result.values[i][j] = 0;
            for(int k=0; k<this->cols; k++)
                result.values[i][j] +=(this->values[i][k] * rhs.values[k][j]);
        }
    }

    return result;
}

Matrix Matrix::operator*(const double &rhs) const {

    Matrix result(this->rows, this->cols);
    for(int i=0; i<result.rows; i++)
        for(int j=0; j<result.cols; j++)
            result.values[i][j] = this->values[i][j] * rhs;

    return result;
}

Matrix Matrix::operator/(const double &rhs) const {
    return (*this)*(1/rhs);
}

const double & Matrix::operator()(const int &row, const int &col) const {
    if(row<0 && row>=this->rows) throw std::string("Out of bound row index");
    if(col<0 && col>=this->cols) throw std::string("Out of bound col index");

    return this->values[row][col];
}

double & Matrix::operator()(const int &row, const int &col) {
    if(row<0 && row>=this->rows) throw std::string("Out of bound row index");
    if(col<0 && col>=this->cols) throw std::string("Out of bound col index");

    return this->values[row][col];
}

const int & Matrix::getRows() const {
    return this->rows;
}

const int & Matrix::getCols() const {
    return this->cols;
}

Matrix operator*(const double &lhs, const Matrix &rhs) {
    return rhs*lhs;
}

Matrix Matrix::Identity(const int &degree) {
    Matrix identity(degree, degree);
    for(int i=0; i<degree; i++)
        identity(i, i) = 1;
    return identity;
}

double Matrix::Det(const Matrix &input) {
    if(input.getRows()!=input.getCols())
        throw std::string("Try to calculate determinant from non square matrix");

    int n = input.getRows();

    if(n==1) {
        return input(0, 0);
    } else if(n==2) {
        return input(0, 0)*input(1, 1) - input(0, 1)*input(1, 0);
    } else if(n==3) {
        return  input(0, 0)*input(1, 1)*input(2, 2) +
                input(0, 1)*input(1, 2)*input(2, 0) +
                input(0, 2)*input(1, 0)*input(2, 1) -
                input(0, 2)*input(1, 1)*input(2, 0) -
                input(0, 1)*input(1, 0)*input(2, 2) -
                input(0, 0)*input(1, 2)*input(2, 1);
    } else {

        double det = 0;

        for(int i=0; i<n; i++) {
            Matrix tmp(n-1, n-1);
            int index = 0, index_tmp = 0;
            while(index<n*n) {

                if(index%n!=0 && index/n!=i) {
                    tmp(index_tmp%(n-1), index_tmp/(n-1)) = input(index%n, index/n);
                    index_tmp++;
                }

                index++;
            }
            det +=((i%2==0) ? 1:-1)*input(0, i)*Matrix::Det(tmp);
        }

        return det;
    }
}

std::ostream & operator<<(std::ostream &lhs, const Matrix &rhs) {
    lhs.precision(2);
    lhs.setf(std::ios_base::fixed);

    lhs << "[";
    for(int i=0; i<rhs.getRows(); i++) {
        if(i!=0)
            lhs << " ";
        lhs << "[";
        for(int j=0; j<rhs.getCols(); j++) {
            lhs << rhs(i, j);
            if(j!=rhs.getCols()-1)
                lhs << ",\t";
        }
        lhs << "]";
        if(i!=rhs.getRows()-1)
            lhs << "," << std::endl;
    }
    lhs << "]";

    lhs.unsetf(std::ios_base::fixed);

    return lhs;
}

