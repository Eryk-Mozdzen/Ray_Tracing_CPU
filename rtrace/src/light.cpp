#include <rtrace/light.h>

rtrace::Light::Light() {}

rtrace::Light::Light(const rtrace::Vector3 &position) {
    this->position = position;
}

const rtrace::Vector3 & rtrace::Light::getPosition() const {
    return this->position;
}
