#include "utils.h"

rtrace::Ray::Ray() {}

rtrace::Ray::Ray(const rtrace::Vector3 &origin, const rtrace::Vector3 &direction) {
    this->origin = origin;
    this->direction = direction;
}

rtrace::Light::Light() {}

rtrace::Light::Light(const rtrace::Vector3 &position) {
    this->position = position;
}

const rtrace::Vector3 & rtrace::Light::getPosition() const {
    return this->position;
}

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

rtrace::CollisionData::CollisionData() {
    this->normal = Vector3(1, 0, 0);
    this->distance = 1000000;
    this->exist = false;
}

rtrace::CollisionData rtrace::CollisionData::min(const rtrace::CollisionData &a, const rtrace::CollisionData &b) {
    return (a.distance<b.distance) ? a : b;
}

rtrace::CollisionData rtrace::CollisionData::smin(const rtrace::CollisionData &a, const rtrace::CollisionData &b, const double &k) {
    CollisionData data = CollisionData::min(a, b);

    const double h = std::max(k - std::abs(a.distance-b.distance), 0.)/k;
    data.distance = std::min(a.distance, b.distance) - h*h*h*k*0.167;

    if(data.distance<rtrace::EPSILON) {
        data.exist = true;
    }

    if(h>0) {
        const double m = a.distance/(a.distance + b.distance);
		
		data.material.setColor(color_interpolation(a.material.getColor(), b.material.getColor(), m));
    }
    
    return data;
}

std::ostream & rtrace::operator<<(std::ostream &lhs, const sf::Color &rhs) {
    lhs << "(";
    lhs << (int)rhs.r << "\t";
    lhs << (int)rhs.g << "\t";
    lhs << (int)rhs.b;
    lhs << ")";
    return lhs;
}

sf::Color rtrace::operator*(const double &lhs, const sf::Color &rhs) {
    const double r = lhs*(double)rhs.r;
    const double g = lhs*(double)rhs.g;
    const double b = lhs*(double)rhs.b;

    return sf::Color(
        ((r<255) ? ((r<0) ? 0 : (sf::Uint8)r) : 255),
        ((g<255) ? ((g<0) ? 0 : (sf::Uint8)g) : 255),
        ((b<255) ? ((b<0) ? 0 : (sf::Uint8)b) : 255)
    );
}

sf::Color rtrace::color_interpolation(const sf::Color &color1, const sf::Color &color2, const double &alpha) {
    if(color1==sf::Color::Transparent)
		return color2;
    if(color2==sf::Color::Transparent)
		return color1;

    return sf::Color(
        (1.f-alpha)*color1.r + alpha*color2.r,
        (1.f-alpha)*color1.g + alpha*color2.g,
        (1.f-alpha)*color1.b + alpha*color2.b
    );
}
