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

CollisionData::CollisionData() {
    this->color = sf::Color::Transparent;
    this->distance = 1000000;
    this->exist = false;
}

TextureMenager::TextureMenager() {}

void TextureMenager::load(const std::string &texture_str) {
    sf::Image texture;
    texture.loadFromFile(texture_str);
    textures.push_back(texture);
}

sf::Image* TextureMenager::getTextureReference(const int &index) {
    return &textures[index];
}

std::ostream & operator<<(std::ostream &lhs, const sf::Color &rhs) {
    lhs << "(";
    lhs << (int)rhs.r << "\t";
    lhs << (int)rhs.g << "\t";
    lhs << (int)rhs.b;
    lhs << ")";
    return lhs;
}

sf::Color operator*(const double &lhs, const sf::Color &rhs) {
    return sf::Color(
        std::min(std::max(0., lhs*rhs.r), 255.),
        std::min(std::max(0., lhs*rhs.g), 255.),
        std::min(std::max(0., lhs*rhs.b), 255.)
    );
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




