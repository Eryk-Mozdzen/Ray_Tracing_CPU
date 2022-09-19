#pragma once

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "scene.h"

class Window : public sf::RenderWindow, public rtrace::Scene {
private:
	sf::Image frameBuffer;
	sf::Vector2u renderResolution;
	Mode renderMode;
	unsigned int reflectionDepth;
public:
	Window(const Mode&, const unsigned int&, const unsigned int&, const unsigned int&);
	
	void setReflectionDepth(const unsigned int&);
	void setRenderMode(const Mode&);
	void setRenderResolution(const unsigned int&, const unsigned int&);

	const unsigned int & getReflectionDepth() const;
	const sf::Vector2u & getRenderResolution() const;

	void handleEvents();
	void display(const rtrace::View&);
	void saveFrameToFile(const std::string&) const;
};