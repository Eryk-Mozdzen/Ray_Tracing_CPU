#include "material.h"

Material::Material() : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{255, 0, 0}, 
		texture{nullptr}, textureSize{0, 0}, textureWrap{false} {
	
}

Material::Material(const int &r, const int &g, const int &b) : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{(sf::Uint8)r, (sf::Uint8)g, (sf::Uint8)b}, 
		texture{nullptr}, textureSize{0, 0}, textureWrap{false} {
	
}

Material::Material(sf::Image *texture) : 
		ambient{1}, diffuse{0}, specular{0}, shininess{5}, reflection{0}, color{0, 0, 0}, 
		texture{texture}, textureWrap{true} {

    this->textureSize = texture->getSize();
}

Material::Material(sf::Image *texture, const unsigned int &width, const unsigned int &height) : 
		ambient{1}, diffuse{0}, specular{0}, shininess{5}, reflection{0}, color{0, 0, 0}, 
		texture{texture}, textureSize{width, height}, textureWrap{true} {

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

bool Material::isTexture() const {
    return (this->texture!=nullptr);
}

sf::Color Material::getColor() const {
    assert(!this->isTexture());

    return this->color;
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

