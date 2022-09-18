#pragma once

#include "rtrace.h"

/*  User custom Camera class
    It is used to describes player point of view.   */

class Camera : public rtrace::View {
private:
    const double linearVelocity = 1.5;
    const double angularVelocity = 0.1;
    sf::Vector2i lastMouseCoords;
public:
    Camera(const rtrace::Vector3&, const double&);
    
    void rotate(rtrace::RenderScene&);
    void move();
    void zoomIn();
    void zoomOut();
};
