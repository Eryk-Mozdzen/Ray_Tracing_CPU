#ifndef RAY_TRACING_MATERIAL_H
#define RAY_TRACING_MATERIAL_H

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../include/RayTracingConfig.h"

class Material {
private:
    double ambient, diffuse, specular, shininess; //phong model
    sf::Color color;
    sf::Image *texture;
    sf::Vector2u textureSize;
    bool textureWrap;
public:
    Material();
    Material(const int&);
    Material(const int&, const int&, const int&);
    Material(sf::Image*);
    Material(sf::Image*, const int&, const int&);

    void setParameters(const double&, const double&, const double&, const double&);
    const double & getAmbient() const;
    const double & getDiffuse() const;
    const double & getSpecular() const;
    const double & getShininess() const;

    sf::Color getColorAt(const double&, const double&) const;
};

#endif
