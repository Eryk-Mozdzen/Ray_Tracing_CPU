#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "RayTracing.h"

/*-----------  User custom objects  ---------------*/
#include "Camera.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Torus.h"
#include "Box.h"
#include "Duck.h"

#define WINDOW_WIDTH        1280
#define WINDOW_HEIGHT       720

void drawImage(sf::RenderWindow &window, const sf::Image &image) {
    sf::Texture tex;
    tex.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(tex);
    sprite.scale((double)window.getSize().x/image.getSize().x, (double)window.getSize().y/image.getSize().y);
    
    window.draw(sprite);
}

int main() {

    srand(time(NULL));

    sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ray Tracing");

	window.setMouseCursorVisible(false);
	sf::Vector2i center = sf::Vector2i(window.getSize().x, window.getSize().y)/2;
	sf::Mouse::setPosition(center, window);

	Camera view(Vector3(-40, 20, 20), 1);
	RenderScene scene(SPHERE_TRACING_MODE, 3, 75, 50);
	TextureMenager menager;
	menager.load("../textures/notexture.jpg");

    Duck duck;

    scene.addLightSource(new LightSource(Vector3(0, 0, 40)));

    double scroll = 5;

    while(window.isOpen()) {
        /*-----------  Handle user activity events  ---------------*/
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type==sf::Event::Closed)
                window.close();
            if(event.type==sf::Event::KeyPressed) {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                    window.close();
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
                    scene.setRenderMode(SPHERE_TRACING_MODE);
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                    scene.setRenderMode(RAY_TRACING_MODE);
            }
            if(event.type==sf::Event::MouseWheelScrolled) {
                if(event.mouseWheelScroll.delta>0)  scroll++;
                else                                scroll--;
            }
        }

        /*-----------  Camera move  ---------------*/

        scene.setRenderResolution(15*scroll, 10*scroll);
        //view.setDistanceFromProjectionPlane(scroll/10);

        sf::Vector2i deltaMouse = sf::Mouse::getPosition(window) - center;
        sf::Mouse::setPosition(center, window);
        view.move(deltaMouse);

        /*-----------  Scene objects manipulation  ---------------*/

        scene.clearObjects();

        duck.construct(scene);

        /*-----------  Render  ---------------*/

		clock.restart();

		sf::Image frameBuffer = scene.render(view);

		double renderTime = clock.restart().asSeconds();


        /*-----------  Display window ---------------*/

		std::stringstream windowTitle;
		windowTitle << std::setprecision(5) << std::fixed;
		windowTitle << "Ray Tracing";
		windowTitle << " | Resolution: " << scene.getRenderResolutionWidth() << "x" << scene.getRenderResolutionHeight();
        windowTitle << " | Render Time: " << renderTime << "s";
		windowTitle << " | FPS: " << 1/renderTime;
		windowTitle << " | Zoom: " << view.getDistanceFromProjectionPlane();
		window.setTitle(windowTitle.str());

		window.clear(sf::Color::Green);

		frameBuffer.flipVertically();
		frameBuffer.flipHorizontally();
        drawImage(window, frameBuffer);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            frameBuffer.saveToFile("saved_frame.jpg");
            std::cout << "Saved frame" << std::endl;
		}

        window.display();
    }

}
