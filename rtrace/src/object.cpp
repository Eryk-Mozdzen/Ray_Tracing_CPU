#include <rtrace/object.h>

const rtrace::Transform3 & rtrace::Object::getTransform() const {
	return this->transform;
}

const rtrace::Material & rtrace::Object::getMaterial() const {
	return this->material;
}

void rtrace::Object::setTransform(const rtrace::Transform3 &tr) {
	this->transform = tr;
}

void rtrace::Object::setMaterial(const rtrace::Material &mat) {
	this->material = mat;
}
