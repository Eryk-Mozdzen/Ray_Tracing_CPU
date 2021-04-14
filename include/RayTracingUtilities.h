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
    Ray(const Vector3&, const Vector3&);
    bool intersect(const Vector3&, double*) const;
};

class Transform3 : public Matrix {
private:
    Matrix inv;
    bool invReady;
public:
    Transform3();
    Transform3 & operator=(const Matrix &);

    void translate(const Vector3 &d);
    void rotate(const Vector3&, const double&);

    Vector3 getRelativeToTransform(const Vector3&);
    Vector3 getRelativeToReferenceFrame(const Vector3&);

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

    virtual bool intersect(const Ray&, CollisionData*) {
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
