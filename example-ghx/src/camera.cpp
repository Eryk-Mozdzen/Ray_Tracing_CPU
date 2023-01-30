#include "camera.h"

Camera::Camera(rtrace::Vector3 position, sf::WindowBase &window) : rtrace::View(position, 1), relativeTo{window}, 
		center{static_cast<sf::Vector2i>(relativeTo.getSize()/2u)} {
			
	window.setMouseCursorVisible(false);

	sf::Mouse::setPosition(center, relativeTo);
}

void Camera::rotation() {
	constexpr double angularVelocity = 0.1;

    const sf::Vector2i delta = sf::Mouse::getPosition(relativeTo) - center;

    if(std::abs(delta.x)>5) {
        View::rotate(rtrace::Vector3::Z, angularVelocity*((delta.x<0)? 1 : -1));
	}

    if(std::abs(delta.y)>5) {
        View::rotate(rtrace::Vector3::Y, angularVelocity*((delta.y>0)? 1 : -1));
	}

    const double s = View::getDirection(rtrace::Vector3::Y)*rtrace::Vector3::Z;

    if(std::abs(s)>0.1) {
        View::rotate(rtrace::Vector3::X, angularVelocity*((s<0)? 1 : -1));
	}

	sf::Mouse::setPosition(center, relativeTo);
}

void Camera::translation() {
	constexpr double linearVelocity = 1.5;

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

    View::translate(linearVelocity*rtrace::normalize(translation));
}

void Camera::move() {
	rotation();
	translation();
}
