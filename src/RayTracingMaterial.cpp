#include "../include/RayTracingMaterial.h"

Material::Material() {
    this->color = sf::Color::White;
    this->texture = nullptr;
    this->textureWrap = false;
    this->textureSize = sf::Vector2u(0, 0);

    this->setAmbient(0.4);
    this->setDiffuse(0.2);
    this->setSpecular(0.2);
    this->setShininess(5);
    this->setReflection(0.5);
}

Material::Material(const int &r, const int &g, const int &b) : Material() {
    this->color = sf::Color(r, g, b);
}

Material::Material(sf::Image *texture) : Material() {
    this->texture = texture;
    this->textureSize = texture->getSize();
}

Material::Material(sf::Image *texture, const int &width, const int &height) : Material() {
    this->texture = texture;
    this->textureWrap = true;
    this->textureSize = sf::Vector2u(width, height);
}

void Material::setAmbient(const double &ambient) {
    this->ambient = ambient;
}

void Material::setDiffuse(const double &diffuse) {
    this->diffuse = diffuse;
}

void Material::setSpecular(const double &specular) {
    this->specular = specular;
}

void Material::setShininess(const double &shininess) {
    this->shininess = shininess;
}

void Material::setReflection(const double &reflection) {
    this->reflection = reflection;
}

const double & Material::getAmbient() const {
    return this->ambient;
}

const double & Material::getDiffuse() const {
    return this->diffuse;
}

const double & Material::getSpecular() const {
    return this->specular;
}

const double & Material::getShininess() const {
    return this->shininess;
}

const double & Material::getReflection() const {
    return this->reflection;
}

sf::Color Material::getColorAt(const double &x, const double &y) const {
    if(this->texture==nullptr)
        return this->color;

    if(this->textureWrap) {
        sf::Vector2f textCoords(
            std::fmod(x*texture->getSize().x, this->textureSize.x),
            std::fmod(y*texture->getSize().y, this->textureSize.y)
        );

        if(textCoords.x<-EPSILON) textCoords.x +=this->textureSize.x;
        if(textCoords.y<-EPSILON) textCoords.y +=this->textureSize.y;

        //std::cout << textCoords.x << "\t" << x << "\t" << this->texture->getSize().x << std::endl;

        return texture->getPixel(
            (textCoords.x/this->textureSize.x)*this->texture->getSize().x,
            (textCoords.y/this->textureSize.y)*this->texture->getSize().y
        );
    }

    return texture->getPixel(
        x*texture->getSize().x,
        y*texture->getSize().y
    );
}

