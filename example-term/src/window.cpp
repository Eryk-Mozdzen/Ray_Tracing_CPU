#include "window.h"

Window::Window(int width, int height) : width{width}, height{height} {
	initscr();
}

Window::~Window() {
	endwin();
}

char Window::mapColor(const rtrace::Color &color) {

	const rtrace::Vector3 vec(color.r, color.g, color.b);
	const double brightness = rtrace::length(vec);

	if(brightness>200) {
		return '@';
	}
	
	if(brightness>150) {
		return '$';
	}

	if(brightness>100) {
		return '*';
	}

	if(brightness>50) {
		return '.';
	}

	return ' ';
}

void Window::display(const rtrace::View &view) {
  
    std::vector<rtrace::Color> frame = renderRayTracing(view, width, height, 3);

	for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
			mvaddch(i, j, mapColor(frame[i*width + j]));
		}
	}

	refresh();
}
