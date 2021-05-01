#include "Camera.h"

Camera::Camera(const Vector3 &position, const double &distance) : View(position, distance) {

}

void Camera::move(const sf::Vector2i &mouse) {
    Vector3 translation(0, 0, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))         translation +=Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))         translation -=Vector3::UnitX();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))         translation +=Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))         translation -=Vector3::UnitY();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))    translation +=Vector3::UnitZ();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))  translation -=Vector3::UnitZ();

    this->translate(this->linearVelocity*normalize(translation));

    if(std::abs(mouse.x)>5)
        this->rotate(Vector3::UnitZ(), this->angularVelocity*((mouse.x<0)? 1 : -1));

    if(std::abs(mouse.y)>5)
        this->rotate(Vector3::UnitY(), this->angularVelocity*((mouse.y>0)? 1 : -1));

    double s = this->getDirectionY()*Vector3::UnitZ();
    if(std::abs(s)>0.1)
        this->rotate(Vector3::UnitX(), this->angularVelocity*((s<0)? 1 : -1));
}
