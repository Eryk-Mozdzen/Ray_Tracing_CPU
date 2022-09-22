#pragma once

#include <ncurses.h>
#include <rtrace/scene.h>

class Window : public rtrace::Scene {
private:
	int width, height;

	static char mapColor(const rtrace::Color &);
public:
	Window(int, int);
	~Window();

	void display(const rtrace::View&);
};