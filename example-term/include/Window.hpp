#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <rtrace/Scene.hpp>

class Window : public rtrace::Scene {
    int width, height;

    static char mapColor(const rtrace::Color &);

public:
    Window(int, int);
    ~Window();

    void display(const rtrace::View &);
};

#endif
