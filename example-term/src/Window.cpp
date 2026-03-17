#include <ncurses.h>

#include "Window.hpp"

static char map(const rtrace::Color &color) {
    const rtrace::Vector3 vec(color.r, color.g, color.b);
    const double brightness = rtrace::length(vec);

    if(brightness > 200) {
        return '@';
    }

    if(brightness > 150) {
        return '$';
    }

    if(brightness > 100) {
        return '*';
    }

    if(brightness > 50) {
        return '.';
    }

    return ' ';
}

Window::Window(const int width, const int height) : width{width}, height{height} {
    frame.resize(width * height);
    initscr();
}

Window::~Window() {
    endwin();
}

void Window::display(const rtrace::View &view) {
    renderRayTracing(frame, view, width, height, 1);

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            mvaddch(i, j, map(frame[i * width + j]));
        }
    }

    refresh();
}
