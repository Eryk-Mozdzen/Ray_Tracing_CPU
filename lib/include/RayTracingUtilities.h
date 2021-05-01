#ifndef RAY_TRACING_UTILITIES_H
#define RAY_TRACING_UTILITIES_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#include <SFML/Graphics.hpp>

#include "Vector.h"
#include "Matrix.h"

#include "RayTracingMaterial.h"

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

    static CollisionData min(const CollisionData&, const CollisionData&);
    static CollisionData smin(const CollisionData&, const CollisionData&, const double&);
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
    virtual ~Object() {};
    virtual CollisionData intersect(const Ray&) const { return CollisionData(); }     // for ray tracing
    virtual CollisionData distance(const Vector3&) const { return CollisionData(); }  // for sphere tracing
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
