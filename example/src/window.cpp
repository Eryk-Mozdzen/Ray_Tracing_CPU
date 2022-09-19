#include "window.h"

Window::Window(const rtrace::Scene::Mode &mode, 
                            const int &depth, 
                            const int &width, 
                            const int &height) : 
									sf::RenderWindow{sf::VideoMode(1280, 720), "Window"},
									depth{depth},
									width{width}, height{height},
									mode{mode} {

	this->buffer.create(this->width, this->height, sf::Color::Black);

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
                this->mode = rtrace::Scene::SPHERE_TRACING;
			}

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                this->mode = rtrace::Scene::RAY_TRACING;
			}

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				this->buffer.saveToFile("../screenshots/screenshot.jpg");
            }
        }

        if(event.type==sf::Event::MouseWheelScrolled) {
            if(event.mouseWheelScroll.delta>0) {
				this->width +=15;
				this->height +=10;
			} else {
				this->width -=15;
				this->height -=10;
			}

			this->buffer.create(this->width, this->height, sf::Color::Black);
        }
    }
}

void Window::display(const rtrace::View &view) {
    sf::Clock clock;
    clock.restart();

    const std::vector<rtrace::Color> frame = this->render(view, this->width, this->height, this->mode, this->depth);

    const double renderTime = clock.restart().asSeconds();

	for(int i=0; i<this->height; i++) {
        for(int j=0; j<this->width; j++) {
			this->buffer.setPixel(
				j,
				i,
				sf::Color(
					frame[this->width*i + j].r,
					frame[this->width*i + j].g,
					frame[this->width*i + j].b
				)
			);
		}
	}

    sf::Texture tex;
    tex.loadFromImage(this->buffer);

    sf::Sprite sprite(tex);
    sprite.scale((double)this->getSize().x/this->buffer.getSize().x, (double)this->getSize().y/this->buffer.getSize().y);
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
    switch(this->mode) {
        case RAY_TRACING:      title << "ray tracing";	break;
        case SPHERE_TRACING:   title << "sphere tracing"; break;
    }

    sf::RenderWindow::setTitle(title.str());
}
