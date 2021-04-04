#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#include <vector>

class Matrix {
private:
    std::vector<std::vector<double>> values;
    int rows, cols;
public:
    Matrix();
    Matrix(const int &, const int &);

    Matrix transposition() const;
    Matrix inverse() const;

    Matrix operator+(const Matrix &) const;
    Matrix operator-(const Matrix &) const;
    Matrix operator*(const Matrix &) const;
    Matrix operator*(const double &) const;
    Matrix operator/(const double &) const;

    const double & operator()(const int &, const int &) const;
    double & operator()(const int &, const int &);
    const int & getRows() const;
    const int & getCols() const;

    static Matrix Inverse(const Matrix &);
    static Matrix Identity(const int &);
    static double Det(const Matrix &);
};

Matrix operator*(const double &, const Matrix &);

std::ostream & operator<<(std::ostream &, const Matrix &);

#endif
