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
    Vector3 position;
    Vector3 directionX, directionY, directionZ;
    double distance;
    Plane screen;
    std::vector<Ray> rays;

    Camera();
    Camera(Vector3 _position, Vector3 _directionX, double _distance);
    void move(sf::Vector2i);
    void update_rays();
};


class Scene {
public:
    std::vector<Object*> objects;
    Camera *camera;

    Scene();
    Scene(Camera*);
    sf::Color trace(Ray, int);
    void add(Object*);
    void clear();
    void render(sf::RenderWindow &);
};


#endif
