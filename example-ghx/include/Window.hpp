#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>

#include <rtrace/Scene.hpp>

class Window : public sf::RenderWindow, public rtrace::Scene {
    std::vector<rtrace::Color> frame;
    int width;
    int height;
    bool mode;
    const int depth;
    sf::Image buffer;

public:
    Window(const int width, const int height, const int depth);

    void handleEvents();
    void display(const rtrace::View &view);
};

#endif
