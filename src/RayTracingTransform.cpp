#include "../include/RayTracingTransform.h"

Transform3::Transform3() : Matrix(4, 4) {
    *this = Matrix::Identity(4);
}

Transform3 & Transform3::operator=(const Matrix &rhs) {
    if(rhs.getRows()!=4) throw std::string("Can't assign matrix to transform");
    if(rhs.getCols()!=4) throw std::string("Can't assign matrix to transform");

    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            (*this)(i, j) = rhs(i, j);

    return *this;
}

void Transform3::translate(const Vector3 &d) {
    Transform3 transform;

    transform(0, 3) = d.x;
    transform(1, 3) = d.y;
    transform(2, 3) = d.z;

    *this = (*this)*transform;
}

void Transform3::rotate(const Vector3 &axis, const double &theta) {
    Transform3 transform;

    Matrix m(3, 3);
    m(0, 1) = -axis.z;
    m(0, 2) = axis.y;
    m(1, 0) = axis.z;
    m(1, 2) = -axis.x;
    m(2, 0) = -axis.y;
    m(2, 1) = axis.x;

    Matrix rot = m*std::sin(theta) + (Matrix::Identity(3) - axis*axis.getTransposition())*std::cos(theta) + (axis*axis.getTransposition());

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            transform(i, j) = rot(i, j);

    *this = (*this)*transform;
}

Vector3 Transform3::getRelativeToTransform(const Vector3 &v) const {
    Matrix p(4, 1);
    p(0, 0) = v.x;
    p(1, 0) = v.y;
    p(2, 0) = v.z;
    p(3, 0) = 1;

    //not good approach (work but highly not efficient)
    //Matrix rel = this->getInverse()*p;

    Matrix rel = solveLinearSystem(*this, p);

    //return v - this->getTranslation();

    return Vector3(
        rel(0, 0),
        rel(1, 0),
        rel(2, 0)
    );
}

Vector3 Transform3::getRelativeToReferenceFrame(const Vector3 &v) const {
    Matrix p(4, 1);
    p(0, 0) = v.x;
    p(1, 0) = v.y;
    p(2, 0) = v.z;
    p(3, 0) = 1;

    Matrix rel = (*this)*p;

    return Vector3(
        rel(0, 0),
        rel(1, 0),
        rel(2, 0)
    );
}

Vector3 Transform3::getTranslation() const {
    Vector3 result;
    result.x = (*this)(0, 3);
    result.y = (*this)(1, 3);
    result.z = (*this)(2, 3);
    return result;
}

Matrix Transform3::getRotation() const {
    Matrix result(3, 3);
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            result(i, j) = (*this)(i, j);
    return result;
}
