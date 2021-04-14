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

class Camera {
public:
    double distance;
    Transform3 transform;
    std::vector<Ray> rays;

    Camera();
    Camera(const Vector3&, const double &);
    void update_rays();
    Vector3 getDirectionX();
    Vector3 getDirectionY();
    Vector3 getDirectionZ();
};

class Scene {
private:
    std::vector<Object*> objects;
public:
    Scene();
    void add(Object*);
    void clear();
    sf::Color trace(const Ray&, const int&) const;
    void render(const Camera&, sf::RenderWindow &) const;
};


#endif
