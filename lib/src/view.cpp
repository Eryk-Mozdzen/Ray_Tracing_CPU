#include "view.h"

View::View() {
    this->distance = 1;
}

View::View(const Vector3 &position, const double &distance) {
    this->distance = distance;

    this->transform.translate(position);
}

Vector3 View::getDirectionX() const {
    return normalize(this->transform.getRelativeToReferenceFrame(Vector3::UnitX()) - this->transform.getTranslation());
}

Vector3 View::getDirectionY() const {
    return normalize(this->transform.getRelativeToReferenceFrame(Vector3::UnitY()) - this->transform.getTranslation());
}

Vector3 View::getDirectionZ() const {
    return normalize(this->transform.getRelativeToReferenceFrame(Vector3::UnitZ()) - this->transform.getTranslation());
}

Vector3 View::getPosition() const {
    return this->transform.getTranslation();
}

const Transform3 & View::getTransform() const {
    return this->transform;
}

double View::getDistanceFromProjectionPlane() const {
    return this->distance;
}

void View::setDistanceFromProjectionPlane(const double &distance) {
    this->distance = distance;
}

void View::translate(const Vector3 &transltion) {
    this->transform.translate(transltion);
}

void View::rotate(const Vector3 &axis, const double &theta) {
    this->transform.rotate(axis, theta);
}



