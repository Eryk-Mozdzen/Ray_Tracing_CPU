#include "material.h"

rtrace::Material::Material() : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{255, 0, 0} {
	
}

rtrace::Material::Material(const unsigned int &r, const unsigned int &g, const unsigned int &b) : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{(sf::Uint8)r, (sf::Uint8)g, (sf::Uint8)b} {
	
}

void rtrace::Material::setAmbient(const double &ambient) {
    this->ambient = ambient;
}

void rtrace::Material::setDiffuse(const double &diffuse) {
    this->diffuse = diffuse;
}

void rtrace::Material::setSpecular(const double &specular) {
    this->specular = specular;
}

void rtrace::Material::setShininess(const double &shininess) {
    this->shininess = shininess;
}

void rtrace::Material::setReflection(const double &reflection) {
    this->reflection = reflection;
}

void rtrace::Material::setColor(const sf::Color &color) {
	this->color = color;
}

void rtrace::Material::setColor(const unsigned int &r, const unsigned int &g, const unsigned int &b) {
	this->color = sf::Color(r, g, b);
}

const double & rtrace::Material::getAmbient() const {
    return this->ambient;
}

const double & rtrace::Material::getDiffuse() const {
    return this->diffuse;
}

const double & rtrace::Material::getSpecular() const {
    return this->specular;
}

const double & rtrace::Material::getShininess() const {
    return this->shininess;
}

const double & rtrace::Material::getReflection() const {
    return this->reflection;
}

sf::Color rtrace::Material::getColor() const {
    return this->color;
}
