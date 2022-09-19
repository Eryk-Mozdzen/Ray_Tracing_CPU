#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <rtrace/view.h>

/*  User custom Camera class
    It is used to describes player point of view.   */

class Camera : public rtrace::View {
private:
    sf::Vector2i lastMouseCoords;
public:
    Camera(const rtrace::Vector3&, const double&);
    
    void rotate(sf::RenderWindow&);
    void move();
    void zoomIn();
    void zoomOut();
};
