#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include "Matrix.h"

class Vector3 {
public:
    double x, y, z;

    Vector3();
    Vector3(const double &, const double &, const double &);
    Vector3(const Matrix &);

    double abs() const;
    Vector3 normalized() const;
    Matrix transposition() const;

    bool operator!=(const Vector3 &) const;
    Vector3 & operator+=(const Vector3 &);
    Vector3 & operator-=(const Vector3 &);
    Vector3 operator+(const Vector3 &) const;
    Vector3 operator-(const Vector3 &) const;
    Vector3 operator^(const Vector3 &) const;
    double operator*(const Vector3 &) const;
    Matrix operator*(const Matrix &) const;
    Vector3 operator*(const double &) const;
    Vector3 operator/(const double &) const;
};

Vector3 operator*(const double &, const Vector3 &);
Vector3 operator*(const Matrix &, const Vector3 &);

double length(const Vector3 &);
Vector3 normalize(const Vector3 &);
Vector3 rotate(const Vector3 &, const Vector3 &, const double &);
std::ostream & operator<<(std::ostream &, const Vector3 &);

#endif
