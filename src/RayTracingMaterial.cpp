#include "../include/RayTracingMaterial.h"

Material::Material() {
    this->color = sf::Color::White;
    this->texture = nullptr;
    this->textureWrap = false;
    this->textureSize = sf::Vector2u(0, 0);

    this->ambient = 0.1;
    this->diffuse = 0.25;
    this->specular = 0.25;
    this->shininess = 5;
}

Material::Material(const int &r, const int &g, const int &b) : Material() {
    this->color = sf::Color(r, g, b);
}

Material::Material(const int &gray) : Material(gray, gray, gray) {

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

void Material::setParameters(const double &ambient, const double &diffuse, const double &specular, const double &shininess) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
}

const double & Material::getAmbient() const {
    return this->ambient;
}

const double & Material::getDiffuse() const {
    return this->ambient;
}

const double & Material::getSpecular() const {
    return this->ambient;
}

const double & Material::getShininess() const {
    return this->shininess;
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

