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

constexpr auto M_PI = 3.14159265358979323846;

class Ray : public Line {
public:
    Ray();
    Ray(Vector3, Vector3);
    bool intersect(Vector3, double*);
};

class Transform3 : public Matrix {
private:
    Matrix inv;
    bool invReady;
public:
    Transform3();
    Transform3 & operator=(const Matrix &);
    void translate(Vector3 d);
    void rotate(Vector3, double);
    Vector3 getTranslation() const;
    Matrix getRotation() const;
    Matrix getInverse();
};

class CollisionData {
public:
    sf::Color color;
    Vector3 normal;
    Vector3 point;
    double reflectivity;

    CollisionData();
};

class Object {
public:
    Object() {}

    virtual bool intersect(Ray, CollisionData*) {
        return false;
    }
};

class TextureMenager {
public:
    std::vector<sf::Image> textures;

    TextureMenager();
    void load(std::string);
    sf::Image* getTextureReference(int);
};

sf::Color color_interpolation(sf::Color, sf::Color, double);

#endif
