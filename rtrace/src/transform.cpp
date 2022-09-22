#include <rtrace/transform.h>

rtrace::Transform3::Transform3() {
	(*this)(0, 0) = 1;
	(*this)(1, 1) = 1;
	(*this)(2, 2) = 1;
	(*this)(3, 3) = 1;
}

rtrace::Transform3& rtrace::Transform3::operator=(const rtrace::Matrix<4, 4> &matrix) {

    for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {
			(*this)(i, j) = matrix(i, j);
		}
	}

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

    rtrace::Matrix<3, 3> m;
    m(0, 1) = -axis.z;
    m(0, 2) = axis.y;
    m(1, 0) = axis.z;
    m(1, 2) = -axis.x;
    m(2, 0) = -axis.y;
    m(2, 1) = axis.x;

	rtrace::Matrix<3, 3> identity;
	identity(0, 0) = 1;
	identity(1, 1) = 1;
	identity(2, 2) = 1;

    rtrace::Matrix<3, 3> rot = m*std::sin(theta) + (identity - axis*rtrace::transposition(axis))*std::cos(theta) + (axis*rtrace::transposition(axis));

    for(unsigned int i=0; i<3; i++)
        for(unsigned int j=0; j<3; j++)
            transform(i, j) = rot(i, j);

    *this = (*this)*transform;
}

rtrace::Vector3 rtrace::Transform3::getRelativeToTransform(const rtrace::Vector3 &v) const {
    rtrace::Matrix<4, 1> p;
    p(0, 0) = v.x;
    p(1, 0) = v.y;
    p(2, 0) = v.z;
    p(3, 0) = 1;

	rtrace::Matrix rel = solveLinearSystem(*this, p);

    return rtrace::Vector3(
        rel(0, 0),
        rel(1, 0),
        rel(2, 0)
    );
}

rtrace::Vector3 rtrace::Transform3::getRelativeToReferenceFrame(const rtrace::Vector3 &v) const {
    rtrace::Matrix<4, 1> p;
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
	return rtrace::Vector3(
        (*this)(0, 3),
        (*this)(1, 3),
        (*this)(2, 3)
    );
}

rtrace::Matrix<3, 3> rtrace::Transform3::getRotation() const {
    rtrace::Matrix<3, 3> result;

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            result(i, j) = (*this)(i, j);
	
    return result;
}
