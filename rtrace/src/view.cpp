#include <rtrace/view.h>

rtrace::View::View(const rtrace::Vector3 &position, const double &distance) : distance{distance} {
    transform.translate(position);
}

rtrace::Vector3 rtrace::View::getDirectionX() const {
    return rtrace::normalize(transform.getRelativeToReferenceFrame(rtrace::Vector3::X) - transform.getTranslation());
}

rtrace::Vector3 rtrace::View::getDirectionY() const {
    return rtrace::normalize(transform.getRelativeToReferenceFrame(rtrace::Vector3::Y) - transform.getTranslation());
}

rtrace::Vector3 rtrace::View::getDirectionZ() const {
    return rtrace::normalize(transform.getRelativeToReferenceFrame(rtrace::Vector3::Z) - transform.getTranslation());
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
