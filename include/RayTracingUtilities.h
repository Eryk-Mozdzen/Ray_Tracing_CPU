#ifndef RAY_TRACING_UTILITIES_H
#define RAY_TRACING_UTILITIES_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../include/Vector.h"
#include "../include/Matrix.h"

#include "../include/RayTracingMaterial.h"

struct Ray {
    Vector3 origin;
    Vector3 direction;

    Ray();
    Ray(const Vector3&, const Vector3&);
};

struct CollisionData {
    Vector3 normal;
    Vector3 point;
    Material material;
    sf::Color color;
    double distance;
    bool exist;

    CollisionData();
};

class LightSource {
private:
    Vector3 position;
public:
    LightSource();
    LightSource(const Vector3&);
    const Vector3 & getPosition() const;
};

class Object {
public:
    Object() {}

    virtual bool intersect(const Ray&, CollisionData&) const {
        return false;
    }
};

class TextureMenager {
private:
    std::vector<sf::Image> textures;
public:
    TextureMenager();
    void load(const std::string&);
    sf::Image* getTextureReference(const int&);
};

std::ostream & operator<<(std::ostream&, const sf::Color&);
sf::Color operator*(const double&, const sf::Color&);
sf::Color color_interpolation(const sf::Color&, const sf::Color&, const double&);

#endif
