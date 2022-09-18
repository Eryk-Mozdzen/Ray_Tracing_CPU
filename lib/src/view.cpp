#include "view.h"

rtrace::View::View() {
    this->distance = 1;
}

rtrace::View::View(const rtrace::Vector3 &position, const double &distance) {
    this->distance = distance;

    this->transform.translate(position);
}

rtrace::Vector3 rtrace::View::getDirectionX() const {
    return rtrace::normalize(this->transform.getRelativeToReferenceFrame(rtrace::Vector3::UnitX()) - this->transform.getTranslation());
}

rtrace::Vector3 rtrace::View::getDirectionY() const {
    return rtrace::normalize(this->transform.getRelativeToReferenceFrame(rtrace::Vector3::UnitY()) - this->transform.getTranslation());
}

rtrace::Vector3 rtrace::View::getDirectionZ() const {
    return rtrace::normalize(this->transform.getRelativeToReferenceFrame(rtrace::Vector3::UnitZ()) - this->transform.getTranslation());
}

rtrace::Vector3 rtrace::View::getPosition() const {
    return this->transform.getTranslation();
}

const rtrace::Transform3 & rtrace::View::getTransform() const {
    return this->transform;
}

double rtrace::View::getDistanceFromProjectionPlane() const {
    return this->distance;
}

void rtrace::View::setDistanceFromProjectionPlane(const double &distance) {
    this->distance = distance;
}

void rtrace::View::translate(const rtrace::Vector3 &transltion) {
    this->transform.translate(transltion);
}

void rtrace::View::rotate(const rtrace::Vector3 &axis, const double &theta) {
    this->transform.rotate(axis, theta);
}



