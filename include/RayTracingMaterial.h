#ifndef RAY_TRACING_MATERIAL_H
#define RAY_TRACING_MATERIAL_H

#ifndef EPSILON
    #define EPSILON     0.0001
#endif

#include <iostream>
#include <cassert>
#include <cmath>

#include <SFML/Graphics.hpp>

class Material {
private:
    double ambient, diffuse, specular, shininess, reflection; //phong model
    sf::Color color;
    sf::Image *texture;
    sf::Vector2u textureSize;
    bool textureWrap;
public:
    Material();
    Material(const int&, const int&, const int&);
    Material(sf::Image*);
    Material(sf::Image*, const int&, const int&);

    void setAmbient(const double&);
    void setDiffuse(const double&);
    void setSpecular(const double&);
    void setShininess(const double&);
    void setReflection(const double&);

    const double & getAmbient() const;
    const double & getDiffuse() const;
    const double & getSpecular() const;
    const double & getShininess() const;
    const double & getReflection() const;

    bool isTexture() const;

    sf::Color getColor() const;
    sf::Color getColorAt(const double&, const double&) const;
};

#endif
