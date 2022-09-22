#include "camera.h"

constexpr double linearVelocity = 1.5;
constexpr double angularVelocity = 0.1;

Camera::Camera(rtrace::Vector3 position) : rtrace::View(position, 1) {
    lastMouseCoords = sf::Mouse::getPosition();
}

void Camera::rotate(sf::RenderWindow &window) {
    sf::Vector2i deltaMouse = sf::Mouse::getPosition(window) - lastMouseCoords;

    if(std::abs(deltaMouse.x)>5) {
        View::rotate(rtrace::Vector3::Z, angularVelocity*((deltaMouse.x<0)? 1 : -1));
	}

    if(std::abs(deltaMouse.y)>5) {
        View::rotate(rtrace::Vector3::Y, angularVelocity*((deltaMouse.y>0)? 1 : -1));
	}

    double s = this->getDirectionY()*rtrace::Vector3::Z;
    if(std::abs(s)>0.1) {
        View::rotate(rtrace::Vector3::X, angularVelocity*((s<0)? 1 : -1));
	}

	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x/2, window.getSize().y/2), window);

	lastMouseCoords = sf::Mouse::getPosition(window);
}

void Camera::move() {
    rtrace::Vector3 translation;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		translation +=rtrace::Vector3::X;
	}
	
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		translation -=rtrace::Vector3::X;
	}

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		translation +=rtrace::Vector3::Y;
	}

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		translation -=rtrace::Vector3::Y;
	}

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		translation +=rtrace::Vector3::Z;
	}

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		translation -=rtrace::Vector3::Z;
	}

    translate(linearVelocity*rtrace::normalize(translation));
}
