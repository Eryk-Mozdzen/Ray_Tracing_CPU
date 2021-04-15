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
public:
    double distance;
    Transform3 transform;

    View();
    View(const Vector3&, const double &);
    Vector3 getDirectionX() const;
    Vector3 getDirectionY() const;
    Vector3 getDirectionZ() const;
};

class Scene {
private:
    std::vector<Object*> objects;
public:
    Scene();
    void add(Object*);
    void clear();
    sf::Color trace(const Ray&, const int&) const;
    sf::Image render(const View&, const int&, const int&) const;
};


#endif
