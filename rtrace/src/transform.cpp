#include <rtrace/transform.h>

rtrace::Transform3::Transform3() {
	rotation(0, 0) = 1;
	rotation(1, 1) = 1;
	rotation(2, 2) = 1;
}

void rtrace::Transform3::translate(const rtrace::Vector3 &vec) {
   	translation +=rotation*vec;
}

void rtrace::Transform3::rotate(const rtrace::Vector3 &axis, const double &theta) {
	const rtrace::Vector3 u = rtrace::normalize(axis);
	const double s = std::sin(theta);
	const double c = std::cos(theta);

	rtrace::Matrix33 rot;

	rot(0, 0) = c + u.x*u.x*(1 - c);
	rot(0, 1) = u.x*u.y*(1 - c) - u.z*s;
	rot(0, 2) = u.x*u.z*(1 - c) + u.y*s;

	rot(1, 0) = u.y*u.z*(1 - c) + u.z*s;
	rot(1, 1) = c + u.y*u.y*(1 - c);
	rot(1, 2) = u.y*u.z*(1 - c) - u.x*s;

	rot(2, 0) = u.z*u.x*(1 - c) - u.y*s;
	rot(2, 1) = u.z*u.y*(1 - c) + u.x*s;
	rot(2, 2) = c + u.z*u.z*(1 - c);

    rotation = rotation*rot;
}

rtrace::Vector3 rtrace::Transform3::convertWordToFrame(const rtrace::Vector3 &vec) const {
	return rtrace::solveLinearSystem(rotation, vec - translation);
}

rtrace::Vector3 rtrace::Transform3::convertFrameToWord(const rtrace::Vector3 &vec) const {
	return rotation*vec + translation;
}

const rtrace::Vector3 & rtrace::Transform3::getTranslation() const {
	return translation;
}

const rtrace::Matrix33 & rtrace::Transform3::getRotation() const {
    return rotation;
}
