#include "transform.h"

rtrace::Transform3::Transform3() : rtrace::Matrix(4, 4) {
    *this = Matrix::Identity(4);
}

rtrace::Transform3 & rtrace::Transform3::operator=(const rtrace::Matrix &rhs) {
    if(rhs.getRows()!=4) throw std::string("Can't assign matrix to transform");
    if(rhs.getCols()!=4) throw std::string("Can't assign matrix to transform");

    for(unsigned int i=0; i<4; i++)
        for(unsigned int j=0; j<4; j++)
            (*this)(i, j) = rhs(i, j);

    return *this;
}

void rtrace::Transform3::translate(const rtrace::Vector3 &d) {
    Transform3 transform;

    transform(0, 3) = d.x;
    transform(1, 3) = d.y;
    transform(2, 3) = d.z;

    *this = (*this)*transform;
}

void rtrace::Transform3::rotate(const rtrace::Vector3 &a, const double &theta) {
	const rtrace::Vector3 axis = normalize(a);

    rtrace::Transform3 transform;

    rtrace::Matrix m(3, 3);
    m(0, 1) = -axis.z;
    m(0, 2) = axis.y;
    m(1, 0) = axis.z;
    m(1, 2) = -axis.x;
    m(2, 0) = -axis.y;
    m(2, 1) = axis.x;

    rtrace::Matrix rot = m*std::sin(theta) + (rtrace::Matrix::Identity(3) - axis*axis.getTransposition())*std::cos(theta) + (axis*axis.getTransposition());

    for(unsigned int i=0; i<3; i++)
        for(unsigned int j=0; j<3; j++)
            transform(i, j) = rot(i, j);

    *this = (*this)*transform;
}

rtrace::Vector3 rtrace::Transform3::getRelativeToTransform(const rtrace::Vector3 &v) const {
    rtrace::Matrix p(4, 1);
    p(0, 0) = v.x;
    p(1, 0) = v.y;
    p(2, 0) = v.z;
    p(3, 0) = 1;

    //not good approach (work but highly not efficient)
    //Matrix rel = this->getInverse()*p;

    rtrace::Matrix rel = solveLinearSystemCramersRule(*this, p);
    //Matrix rel = solveLinearSystemJacobiMethod(*this, p);

    //return v - this->getTranslation();

    return rtrace::Vector3(
        rel(0, 0),
        rel(1, 0),
        rel(2, 0)
    );
}

rtrace::Vector3 rtrace::Transform3::getRelativeToReferenceFrame(const rtrace::Vector3 &v) const {
    rtrace::Matrix p(4, 1);
    p(0, 0) = v.x;
    p(1, 0) = v.y;
    p(2, 0) = v.z;
    p(3, 0) = 1;

    rtrace::Matrix rel = (*this)*p;

    return Vector3(
        rel(0, 0),
        rel(1, 0),
        rel(2, 0)
    );
}

rtrace::Vector3 rtrace::Transform3::getTranslation() const {
    rtrace::Vector3 result;
    result.x = (*this)(0, 3);
    result.y = (*this)(1, 3);
    result.z = (*this)(2, 3);
    return result;
}

rtrace::Matrix rtrace::Transform3::getRotation() const {
    rtrace::Matrix result(3, 3);
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            result(i, j) = (*this)(i, j);
    return result;
}
