#include "camera.h"

constexpr double linearVelocity = 1.5;
constexpr double angularVelocity = 0.1;

Camera::Camera(const rtrace::Vector3 &position, const double &distance) : rtrace::View(position, distance) {
    this->lastMouseCoords = sf::Mouse::getPosition();
}

void Camera::rotate(sf::RenderWindow &window) {
    window.setMouseCursorVisible(false);

    sf::Vector2i deltaMouse = sf::Mouse::getPosition(window) - this->lastMouseCoords;

    if(std::abs(deltaMouse.x)>5)
        View::rotate(rtrace::Vector3::UnitZ(), angularVelocity*((deltaMouse.x<0)? 1 : -1));

    if(std::abs(deltaMouse.y)>5)
        View::rotate(rtrace::Vector3::UnitY(), angularVelocity*((deltaMouse.y>0)? 1 : -1));

    double s = this->getDirectionY()*rtrace::Vector3::UnitZ();
    if(std::abs(s)>0.1)
        View::rotate(rtrace::Vector3::UnitX(), angularVelocity*((s<0)? 1 : -1));

    sf::Mouse::setPosition(sf::Vector2i(window.getSize().x/2, window.getSize().y/2), window);
    this->lastMouseCoords = sf::Mouse::getPosition(window);
}

void Camera::move() {
    rtrace::Vector3 translation;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))         translation +=rtrace::Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))         translation -=rtrace::Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))         translation +=rtrace::Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))         translation -=rtrace::Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))    translation +=rtrace::Vector3::UnitZ();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))  translation -=rtrace::Vector3::UnitZ();

    this->translate(linearVelocity*rtrace::normalize(translation));
}

void Camera::zoomIn() {
    const double d = this->getDistanceFromProjectionPlane();

    this->setDistanceFromProjectionPlane(d + ((d<1) ? 0.1 : 1));
}

void Camera::zoomOut() {
    const double d = this->getDistanceFromProjectionPlane();

    if(d<=0.1+rtrace::EPSILON)
        return;

    this->setDistanceFromProjectionPlane(d - ((d<=1) ? 0.1 : 1));
}
