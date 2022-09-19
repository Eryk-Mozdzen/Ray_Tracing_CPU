#pragma once

#include "view.h"
#include "rtrace.h"

/*  User custom Camera class
    It is used to describes player point of view.   */

class Camera : public rtrace::View {
private:
    sf::Vector2i lastMouseCoords;
public:
    Camera(const rtrace::Vector3&, const double&);
    
    void rotate(rtrace::Scene&);
    void move();
    void zoomIn();
    void zoomOut();
};
