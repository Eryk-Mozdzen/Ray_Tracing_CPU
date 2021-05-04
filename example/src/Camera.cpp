#include "Camera.h"

Camera::Camera(const Vector3 &position, const double &distance) : View(position, distance) {
    this->lastMouseCoords = sf::Mouse::getPosition();
}

void Camera::rotate(RenderScene &scene) {
    scene.setMouseCursorVisible(false);

    sf::Vector2i deltaMouse = sf::Mouse::getPosition(scene) - this->lastMouseCoords;

    if(std::abs(deltaMouse.x)>5)
        View::rotate(Vector3::UnitZ(), this->angularVelocity*((deltaMouse.x<0)? 1 : -1));

    if(std::abs(deltaMouse.y)>5)
        View::rotate(Vector3::UnitY(), this->angularVelocity*((deltaMouse.y>0)? 1 : -1));

    double s = this->getDirectionY()*Vector3::UnitZ();
    if(std::abs(s)>0.1)
        View::rotate(Vector3::UnitX(), this->angularVelocity*((s<0)? 1 : -1));

    sf::Mouse::setPosition(sf::Vector2i(scene.getSize().x/2, scene.getSize().y/2), scene);
    this->lastMouseCoords = sf::Mouse::getPosition(scene);
}

void Camera::move() {
    Vector3 translation;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))         translation +=Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))         translation -=Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))         translation +=Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))         translation -=Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))    translation +=Vector3::UnitZ();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))  translation -=Vector3::UnitZ();

    this->translate(this->linearVelocity*normalize(translation));
}

void Camera::zoomIn() {
    const double d = this->getDistanceFromProjectionPlane();

    this->setDistanceFromProjectionPlane(d + ((d<1) ? 0.1 : 1));
}

void Camera::zoomOut() {
    const double d = this->getDistanceFromProjectionPlane();

    if(d<=0.1+EPSILON)
        return;

    this->setDistanceFromProjectionPlane(d - ((d<=1) ? 0.1 : 1));
}
