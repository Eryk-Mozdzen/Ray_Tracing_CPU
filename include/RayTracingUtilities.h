#ifndef RAY_TRACING_UTILITIES_H
#define RAY_TRACING_UTILITIES_H

#include <string>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Vector.h"
#include "Matrix.h"
#include "geometry.h"

#include "RayTracingConfig.h"
#include "RayTracingMaterial.h"
#include "RayTracingTransform.h"

constexpr auto M_PI = 3.14159265358979323846;

class Ray : public Line {
public:
    Ray();
    Ray(const Vector3&, const Vector3&);
    bool intersect(const Vector3&, double*) const;
};

class LightSource {
private:
    Vector3 position;
    sf::Color color;
public:
    LightSource();
    LightSource(const Vector3&);
    const Vector3 & getPosition() const;
};

struct CollisionData {
    Vector3 normal;
    Vector3 point;
    Material material;
    sf::Color color;

    CollisionData();
};

class Object {
public:
    Material material;
    Transform3 transform;

    Object() {}

    virtual bool intersect(const Ray&, CollisionData&) {
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

sf::Color color_interpolation(const sf::Color&, const sf::Color&, const double&);

#endif
