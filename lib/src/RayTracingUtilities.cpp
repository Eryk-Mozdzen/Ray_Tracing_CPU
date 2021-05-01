#include "RayTracingUtilities.h"

Ray::Ray() {}

Ray::Ray(const Vector3 &origin, const Vector3 &direction) {
    this->origin = origin;
    this->direction = direction;
}

LightSource::LightSource() {}

LightSource::LightSource(const Vector3 &position) {
    this->position = position;
}

const Vector3 & LightSource::getPosition() const {
    return this->position;
}

CollisionData::CollisionData() {
    this->color = sf::Color::Transparent;
    this->normal = Vector3(1, 0, 0);
    this->distance = 1000000;
    this->exist = false;
}

CollisionData CollisionData::min(const CollisionData &a, const CollisionData &b) {
    return (a.distance<b.distance) ? a : b;
}

CollisionData CollisionData::smin(const CollisionData &a, const CollisionData &b, const double &k) {
    CollisionData data = CollisionData::min(a, b);

    const double h = std::max(k - std::abs(a.distance-b.distance), 0.)/k;
    data.distance = std::min(a.distance, b.distance) - h*h*h*k*0.167;

    if(data.distance<EPSILON) {
        data.exist = true;
    }

    if(h>0) {
        const double m = a.distance/(a.distance + b.distance);
        data.color = color_interpolation(a.color, b.color, m);
    }
    
    return data;
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
    /*return sf::Color(
        std::min(std::max(0., lhs*rhs.r), 255.),
        std::min(std::max(0., lhs*rhs.g), 255.),
        std::min(std::max(0., lhs*rhs.b), 255.)
    );*/

    const double r = lhs*(double)rhs.r;
    const double g = lhs*(double)rhs.g;
    const double b = lhs*(double)rhs.b;

    return sf::Color(
        ((r<255) ? ((r<0) ? 0 : (sf::Uint8)r) : 255),
        ((g<255) ? ((g<0) ? 0 : (sf::Uint8)g) : 255),
        ((b<255) ? ((b<0) ? 0 : (sf::Uint8)b) : 255)
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




