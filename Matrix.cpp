#include "Matrix.h"
#include <string>

Matrix::Matrix() {}

Matrix::Matrix(const int &rows, const int &cols) {
    this->rows = rows;
    this->cols = cols;

    this->values.resize(rows);
    for(int i=0; i<rows; i++)
        this->values[i].resize(cols);
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

const double & Matrix::operator()(const int &row, const int &col) const {
    if(row>=0 && row<this->rows) throw std::string("Out of bound row index");
    if(col>=0 && col<this->cols) throw std::string("Out of bound col index");

    return this->values[row][col];
}

double & Matrix::operator()(const int &row, const int &col) {
    if(row>=0 && row<this->rows) throw std::string("Out of bound row index");
    if(col>=0 && col<this->cols) throw std::string("Out of bound col index");

    return this->values[row][col];
}

const int & Matrix::getRows() const {
    return this->rows;
}

const int & Matrix::getCols() const {
    return this->cols;
}

std::ostream & operator<<(std::ostream &lhs, const Matrix &rhs) {
    lhs << "[";
    for(int i=0; i<rhs.getRows(); i++) {
        lhs << "[";
        for(int j=0; j<rhs.getCols(); j++) {
            lhs << rhs(i, j);
            if(j!=rhs.getCols()-1)
                lhs << ", ";
        }
        lhs << "]" << std::endl;
    }
    lhs << "]";

    return lhs;
}

