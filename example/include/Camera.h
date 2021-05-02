#ifndef CAMERA_H
#define CAMERA_H

#include "RayTracing.h"
#include <SFML/Graphics.hpp>

class Camera : public View {
private:
    const double linearVelocity = 1.5;
    const double angularVelocity = 0.1;
    sf::Vector2i lastMouseCoords;
public:
    Camera(const Vector3&, const double&);
    
    void rotate();
    void move();
    void zoomIn();
    void zoomOut();
};

#endif