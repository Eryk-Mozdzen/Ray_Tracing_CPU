#include "../include/RayTracingUtilities.h"

Ray::Ray() {}

Ray::Ray(const Vector3 &origin, const Vector3 &direction) : Line(origin, direction) {}

bool Ray::intersect(const Vector3 &point, double *t) const {
    double d = distance(Line(this->point, this->direction), point);
    if(d>EPSILON)
        return false;

    *t = direction*(point-this->point);

    return true;
}

LightSource::LightSource() {
    this->position = Vector3(0, 0, 0);
    this->color = sf::Color::White;
}

LightSource::LightSource(const Vector3 &position) {
    this->position = position;
    this->color = sf::Color::White;
}

const Vector3 & LightSource::getPosition() const {
    return this->position;
}

CollisionData::CollisionData() {}

TextureMenager::TextureMenager() {}

void TextureMenager::load(const std::string &texture_str) {
    sf::Image texture;
    texture.loadFromFile(texture_str);
    textures.push_back(texture);
}

sf::Image* TextureMenager::getTextureReference(const int &index) {
    return &textures[index];
}

sf::Color color_interpolation(const sf::Color &color1, const sf::Color &color2, const double &alpha) {
    if(color1==sf::Color::Transparent) return color2;
    if(color2==sf::Color::Transparent) return color1;

    return sf::Color(
        (1.f-alpha)*color1.r + alpha*color2.r,
        (1.f-alpha)*color1.g + alpha*color2.g,
        (1.f-alpha)*color1.b + alpha*color2.b
    );
}




