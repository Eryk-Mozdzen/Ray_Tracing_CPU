#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <rtrace/Scene.hpp>

class Window : public rtrace::Scene {
    std::vector<rtrace::Color> frame;
    const int width;
    const int height;

public:
    Window(const int width, const int height);
    ~Window();

    void display(const rtrace::View &view);
};

#endif
