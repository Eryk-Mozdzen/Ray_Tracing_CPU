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
    Camera(rtrace::Vector3);
    
    void rotate(sf::RenderWindow&);
    void move();
};
