#pragma once

#include <SFML/Window.hpp>
#include <rtrace/view.h>

/*  User custom Camera class
    It is used to describes player point of view and movement.   */

class Camera : public rtrace::View {
private:
	const sf::WindowBase &relativeTo;
	const sf::Vector2i center;
	
	void rotation();
    void translation();
public:
    Camera(rtrace::Vector3, sf::WindowBase &);

	void move();
};
