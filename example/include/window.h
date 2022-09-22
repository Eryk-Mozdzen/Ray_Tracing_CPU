#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <rtrace/scene.h>

class Window : public sf::RenderWindow, public rtrace::Scene {
private:
	int width, height;
	bool mode;
	int depth;
	sf::Image buffer;
public:
	Window(int, int, int);

	void handleEvents();
	void display(const rtrace::View&);
};