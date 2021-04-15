#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Vector.h"
#include "Matrix.h"
#include "geometry.h"

#include "RayTracingConfig.h"
#include "RayTracingUtilities.h"

class View {
private:
    double distance;
    Transform3 transform;
public:
    View();
    View(const Vector3&, const double &);

    Vector3 getDirectionX() const;
    Vector3 getDirectionY() const;
    Vector3 getDirectionZ() const;
    Vector3 getPosition() const;
    const Transform3 & getTransform() const;
    double getDistanceFromProjectionPlane() const;

    void setDistanceFromProjectionPlane(const double&);
    void translate(const Vector3&);
    void rotate(const Vector3&, const double&);
};

class Scene {
private:
    std::vector<Object*> objects;
    std::vector<LightSource*> lights;
public:
    Scene();
    void addObject(Object*);
    void addLightSource(LightSource*);
    sf::Color trace(const Ray&, const int&) const;
    sf::Image render(const View&, const int&, const int&) const;
};

#endif
