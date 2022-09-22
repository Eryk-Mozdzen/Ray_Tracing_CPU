#include <rtrace/view.h>

rtrace::View::View(const rtrace::Vector3 &position, const double &distance) : distance{distance} {
    transform.translate(position);
}

rtrace::Vector3 rtrace::View::getDirection(const rtrace::Vector3 &dir) const {
	return transform.getRotation()*dir;
}

rtrace::Vector3 rtrace::View::getPosition() const {
    return transform.getTranslation();
}

double rtrace::View::getDistanceFromProjectionPlane() const {
    return distance;
}

void rtrace::View::setDistanceFromProjectionPlane(const double &distance) {
    this->distance = distance;
}

void rtrace::View::translate(const rtrace::Vector3 &transltion) {
    transform.translate(transltion);
}

void rtrace::View::rotate(const rtrace::Vector3 &axis, const double &theta) {
    transform.rotate(axis, theta);
}
