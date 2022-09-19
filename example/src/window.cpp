#include "window.h"

Window::Window(const Mode &renderMode, 
                            const unsigned int &reflectionDepth, 
                            const unsigned int &resolutionWidgth, 
                            const unsigned int &resolutionHeight) : 
							sf::RenderWindow{sf::VideoMode(1280, 720), "Render Scene"},
							renderMode{renderMode},
							reflectionDepth{reflectionDepth} {

    this->setRenderResolution(resolutionWidgth, resolutionHeight);
}

void Window::setReflectionDepth(const unsigned int &reflectionDepth) {
    this->reflectionDepth = reflectionDepth;
}

void Window::setRenderMode(const Mode &renderMode) {
    this->renderMode = renderMode;
}

void Window::setRenderResolution(const unsigned int &resolutionH, const unsigned int &resolutionV) {
    this->renderResolution = sf::Vector2u(resolutionH, resolutionV);

    this->frameBuffer.create(resolutionH, resolutionV, sf::Color::Black);
}

const unsigned int & Window::getReflectionDepth() const {
    return this->reflectionDepth;
}

const sf::Vector2u & Window::getRenderResolution() const {
    return this->renderResolution;
}

void Window::handleEvents() {
	sf::Event event;

    while(sf::RenderWindow::pollEvent(event)) {
        if(event.type==sf::Event::Closed)
            sf::RenderWindow::close();
		
        if(event.type==sf::Event::KeyPressed) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                sf::RenderWindow::close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
                this->setRenderMode(rtrace::Scene::SPHERE_TRACING);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                this->setRenderMode(rtrace::Scene::RAY_TRACING);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                this->saveFrameToFile("../screenshots/screenshot_" + std::to_string(time(nullptr)) + ".jpg");
                std::cout << "Saved frame" << std::endl;
            }
        }

        if(event.type==sf::Event::MouseWheelScrolled) {
            sf::Vector2u res = this->getRenderResolution();
            if(event.mouseWheelScroll.delta>0)
				this->setRenderResolution(res.x + 15, res.y + 10);
            else
				this->setRenderResolution(res.x - 15, res.y - 10);
        }
    }
}

void Window::display(const rtrace::View &view) {
    sf::Clock clock;
    clock.restart();

    const std::vector<sf::Color> frame = this->render(view, this->renderResolution.x, this->renderResolution.y, this->renderMode, this->reflectionDepth);

    const double renderTime = clock.restart().asSeconds();

	for(unsigned int i=0; i<this->renderResolution.y; i++) {
        for(unsigned int j=0; j<this->renderResolution.x; j++) {
			this->frameBuffer.setPixel(
				this->renderResolution.x - j - 1,
				this->renderResolution.y - i - 1,
				frame[i*this->renderResolution.x + j]
			);
		}
	}

    sf::Texture tex;
    tex.loadFromImage(this->frameBuffer);

    sf::Sprite sprite;
    sprite.setTexture(tex);
    sprite.scale((double)this->getSize().x/this->frameBuffer.getSize().x, (double)this->getSize().y/this->frameBuffer.getSize().y);
	sf::RenderWindow::draw(sprite);

    std::stringstream windowTitle;
    windowTitle << std::setprecision(3) << std::fixed;
    windowTitle << "Ray Tracing";
    windowTitle << " | Resolution: " << this->renderResolution.x << "x" << this->renderResolution.y;
    windowTitle << " | Render Time: " << renderTime*1000. << "ms";
    windowTitle << " | FPS: " << 1/renderTime;
    windowTitle << " | Zoom: " << view.getDistanceFromProjectionPlane();

    windowTitle << " | Mode: ";
    switch(this->renderMode) {
        case RAY_TRACING:      windowTitle << "ray tracing";	break;
        case SPHERE_TRACING:   windowTitle << "sphere tracing"; break;
        default: break;
    }

    sf::RenderWindow::setTitle(windowTitle.str());
    
    sf::RenderWindow::display();
}

void Window::saveFrameToFile(const std::string &filename) const {
    this->frameBuffer.saveToFile(filename);
}
