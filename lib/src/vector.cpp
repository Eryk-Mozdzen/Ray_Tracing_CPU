#include "vector.h"

rtrace::Vector3::Vector3() : rtrace::Vector3(0, 0, 0) {}

rtrace::Vector3::Vector3(const double &x, const double &y, const double &z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

rtrace::Vector3::Vector3(const Matrix &A) {
    assert(A.getRows()==3);
    assert(A.getCols()==1);

    this->x = A(0, 0);
    this->y = A(1, 0);
    this->z = A(2, 0);
}

double rtrace::Vector3::getLength() const {
    return length(*this);
}

rtrace::Vector3 rtrace::Vector3::getNormalized() const {
    return normalize(*this);
}

rtrace::Matrix rtrace::Vector3::getTransposition() const {
    rtrace::Matrix result(1, 3);

    result(0, 0) = this->x;
    result(0, 1) = this->y;
    result(0, 2) = this->z;

    return result;
}

bool rtrace::Vector3::operator!=(const rtrace::Vector3 &rhs) const {
    return (length(*this - rhs)>0.0001);
}

rtrace::Vector3 & rtrace::Vector3::operator+=(const rtrace::Vector3 &rhs) {
    this->x +=rhs.x;
    this->y +=rhs.y;
    this->z +=rhs.z;

    return *this;
}

rtrace::Vector3 & rtrace::Vector3::operator-=(const rtrace::Vector3 &rhs) {
    this->x -=rhs.x;
    this->y -=rhs.y;
    this->z -=rhs.z;

    return *this;
}

rtrace::Vector3 rtrace::Vector3::operator+(const rtrace::Vector3 &rhs) const {
    return Vector3(
        this->x + rhs.x,
        this->y + rhs.y,
        this->z + rhs.z
    );
}

rtrace::Vector3 rtrace::Vector3::operator-(const rtrace::Vector3 &rhs) const {
    return Vector3(
        this->x - rhs.x,
        this->y - rhs.y,
        this->z - rhs.z
    );
}

rtrace::Matrix rtrace::Vector3::operator*(const rtrace::Matrix &rhs) const {
    return this->getTransposition().getTransposition()*rhs;
}

double rtrace::Vector3::operator*(const rtrace::Vector3 &rhs) const {
    return this->x*rhs.x + this->y*rhs.y + this->z*rhs.z;
}

rtrace::Vector3 rtrace::Vector3::operator^(const rtrace::Vector3 &rhs) const {
    return Vector3(
        this->y*rhs.z - this->z*rhs.y,
        -(this->x*rhs.z - this->z*rhs.x),
        this->x*rhs.y - this->y*rhs.x
    );
}

rtrace::Vector3 rtrace::Vector3::operator*(const double &rhs) const {
    return Vector3(
        this->x * rhs,
        this->y * rhs,
        this->z * rhs
    );
}

rtrace::Vector3 rtrace::Vector3::operator/(const double &rhs) const {
    return (*this)*(1/rhs);
}

rtrace::Vector3 rtrace::Vector3::UnitX() {
    return  rtrace::Vector3(1, 0, 0);
}

rtrace::Vector3 rtrace::Vector3::UnitY() {
    return  rtrace::Vector3(0, 1, 0);
}

rtrace::Vector3 rtrace::Vector3::UnitZ() {
    return  rtrace::Vector3(0, 0, 1);
}

rtrace::Vector3 rtrace::operator*(const double &lhs, const rtrace::Vector3 &rhs) {
    return rhs*lhs;
}

rtrace::Vector3 rtrace::operator*(const rtrace::Matrix &lhs, const rtrace::Vector3 &rhs) {
    return rtrace::Vector3(lhs*(rhs.getTransposition().getTransposition()));
}

rtrace::Vector3 rtrace::operator-(const rtrace::Vector3 &rhs) {
	return rtrace::Vector3(-rhs.x, -rhs.y, -rhs.z);
}

double rtrace::length(const rtrace::Vector3 &vec) {
    return std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2) + std::pow(vec.z, 2));
}

rtrace::Vector3 rtrace::normalize(const rtrace::Vector3 &v) {
    double len = rtrace::length(v);

    if(len==0)
        return v;

    return (v/len);
}

rtrace::Vector3 rtrace::rotate(const rtrace::Vector3 &axis, const rtrace::Vector3 &v, const double &theta) {
    rtrace::Vector3 k = rtrace::normalize(axis);

    double c = std::cos(theta);
    double s = std::sin(theta);

    //Rodrigues' rotation formula
    return (c*v + s*(k^v) + (k*v)*(1.f-c)*k);
}

std::ostream & rtrace::operator<<(std::ostream &lhs, const rtrace::Vector3 &rhs) {
    lhs << "[";
    lhs << rhs.x << ", ";
    lhs << rhs.y << ", ";
    lhs << rhs.z;
    lhs << "]";
    return lhs;
}

