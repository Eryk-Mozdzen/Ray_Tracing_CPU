#include "window.h"

Window::Window(int width, int height, int depth) : 
		sf::RenderWindow{sf::VideoMode(1280, 720), ""},
		width{width}, height{height}, mode{true}, depth{depth} {

	buffer.create(width, height, sf::Color::Black);

	sf::RenderWindow::setMouseCursorVisible(false);
}

void Window::handleEvents() {
	sf::Event event;

    while(sf::RenderWindow::pollEvent(event)) {
        if(event.type==sf::Event::Closed) {
            sf::RenderWindow::close();
		}
		
        if(event.type==sf::Event::KeyPressed) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                sf::RenderWindow::close();
			}

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                mode = false;
			}

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                mode = true;
			}

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				buffer.saveToFile("../screenshots/screenshot.jpg");
            }
        }

        if(event.type==sf::Event::MouseWheelScrolled) {
            if(event.mouseWheelScroll.delta>0) {
				width +=15;
				height +=10;
			} else {
				width -=15;
				height -=10;
			}

			buffer.create(width, height, sf::Color::Black);
        }
    }
}

void Window::display(const rtrace::View &view) {
    sf::Clock clock;
    clock.restart();

    std::vector<rtrace::Color> frame;
	
	if(mode) {
		frame = renderRayTracing(view, width, height, depth);
	} else {
		frame = renderSphereTracing(view, width, height);
	}

    const double renderTime = clock.restart().asSeconds();

	for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
			buffer.setPixel(j, i, sf::Color(
					frame[width*i + j].r,
					frame[width*i + j].g,
					frame[width*i + j].b
				)
			);
		}
	}

    sf::Texture tex;
    tex.loadFromImage(buffer);

    sf::Sprite sprite(tex);
    sprite.scale((double)getSize().x/buffer.getSize().x, (double)getSize().y/buffer.getSize().y);
	sf::RenderWindow::draw(sprite);
	
	sf::RenderWindow::display();

	const double drawTime = clock.restart().asSeconds();

    std::stringstream title;
    title << std::setprecision(3) << std::fixed;
    title << "Resolution: " << this->width << "x" << this->height;
    title << " | Render Time: " << renderTime*1000. << "ms";
	title << " | Draw Time: " << drawTime*1000. << "ms";
    title << " | FPS: " << 1./(renderTime + drawTime);
    title << " | Mode: ";

	if(mode) {
		title << "ray tracing";
	} else {
		title << "sphere tracing";
	}

    sf::RenderWindow::setTitle(title.str());
}
