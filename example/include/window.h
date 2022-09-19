#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <rtrace/scene.h>

class Window : public sf::RenderWindow, public rtrace::Scene {
private:
	int depth, width, height;
	rtrace::Scene::Mode mode;
	sf::Image buffer;
public:
	Window(const rtrace::Scene::Mode&, const int&, const int&, const int&);

	void handleEvents();
	void display(const rtrace::View&);
};