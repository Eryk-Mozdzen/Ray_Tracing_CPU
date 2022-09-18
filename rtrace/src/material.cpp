#include "material.h"

rtrace::Material::Material() : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{255, 0, 0}, 
		texture{nullptr}, textureSize{0, 0}, textureWrap{false} {
	
}

rtrace::Material::Material(const int &r, const int &g, const int &b) : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{(sf::Uint8)r, (sf::Uint8)g, (sf::Uint8)b}, 
		texture{nullptr}, textureSize{0, 0}, textureWrap{false} {
	
}

rtrace::Material::Material(sf::Image *texture) : 
		ambient{1}, diffuse{0}, specular{0}, shininess{5}, reflection{0}, color{0, 0, 0}, 
		texture{texture}, textureWrap{true} {

    this->textureSize = texture->getSize();
}

rtrace::Material::Material(sf::Image *texture, const unsigned int &width, const unsigned int &height) : 
		ambient{1}, diffuse{0}, specular{0}, shininess{5}, reflection{0}, color{0, 0, 0}, 
		texture{texture}, textureSize{width, height}, textureWrap{true} {

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

bool rtrace::Material::isTexture() const {
    return (this->texture!=nullptr);
}

sf::Color rtrace::Material::getColor() const {
    assert(!this->isTexture());

    return this->color;
}

sf::Color rtrace::Material::getColorAt(const double &x, const double &y) const {
    if(this->texture==nullptr)
        return this->color;

    if(this->textureWrap) {
        sf::Vector2f textCoords(
            std::fmod(x*texture->getSize().x, this->textureSize.x),
            std::fmod(y*texture->getSize().y, this->textureSize.y)
        );

        if(textCoords.x<-EPSILON)
			textCoords.x +=this->textureSize.x;
        if(textCoords.y<-EPSILON)
			textCoords.y +=this->textureSize.y;

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

