#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <chrono>

#include <SFML/Window.hpp>

#include <rtrace/View.hpp>

/*  User custom Camera class
    It is used to describes player point of view and movement.  */

class Camera : public rtrace::View {
    const sf::WindowBase &relativeTo;
    const sf::Vector2i center;
    std::chrono::steady_clock::time_point prevTime;

    void rotation();
    void translation(const double dt);

public:
    Camera(const rtrace::Vector3 postion, sf::WindowBase &window);

    void move();
};

#endif
