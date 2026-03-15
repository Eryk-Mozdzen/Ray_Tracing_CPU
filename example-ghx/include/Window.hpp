#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>

#include <rtrace/Scene.hpp>

class Window : public sf::RenderWindow, public rtrace::Scene {
    int width, height;
    bool mode;
    int depth;
    sf::Image buffer;

public:
    Window(int, int, int);

    void handleEvents();
    void display(const rtrace::View &);
};

#endif
