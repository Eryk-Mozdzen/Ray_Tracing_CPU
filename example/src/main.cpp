/*  Ray tracing example program 
    by Eryk Możdżeń, May 2021r.

    Simple usage of implemented features like:
        - definition of user custom class
        - ray tracing rendering for objects with intersect() method
        - sphere tracing rendering for objects with distance() method

    Controls:
        - Mouse         - camera rotation
        - Mouse Scroll  - resolution change
        - WASD          - camera forward, left, backward, right movment
        - LShift, LCtr  - camera up, down movment
        - N/M keys      - toggling between two modes (ray/sphere tracing)
        - Z key         - take a screenshot
        - X key         - exit the application		*/

#include <iostream>     // cout, endl

#include "camera.h"
#include "sphere.h"
#include "plane.h"
#include "torus.h"

void handleEvents(rtrace::Scene &scene, Camera &camera) {
    sf::Event event;

    while(scene.pollEvent(event)) {
        if(event.type==sf::Event::Closed)
            scene.close();
		
        if(event.type==sf::Event::KeyPressed) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                scene.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
                scene.setRenderMode(rtrace::SPHERE_TRACING_MODE);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                scene.setRenderMode(rtrace::RAY_TRACING_MODE);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                scene.saveFrameToFile("../screenshots/screenshot_" + std::to_string(time(nullptr)) + ".jpg");
                std::cout << "Saved frame" << std::endl;
            }
        }

        if(event.type==sf::Event::MouseWheelScrolled) {
            sf::Vector2u res = scene.getRenderResolution();
            if(event.mouseWheelScroll.delta>0)
				scene.setRenderResolution(res.x + 15, res.y + 10);
            else
				scene.setRenderResolution(res.x - 15, res.y - 10);
        }
    }
}

int main() {

    Camera camera(rtrace::Vector3(-50, 0, 25), 1);
    rtrace::Scene scene(rtrace::RAY_TRACING_MODE, 3, 135, 100);

    /*-----------  Scene setup  ---------------*/

    scene.setMouseCursorVisible(false);

	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(rtrace::Vector3(10, 0, 30), 7, rtrace::Material(255, 0, 0));
	std::shared_ptr<Torus> torus = std::make_shared<Torus>(rtrace::Vector3(0, 0, 15), 6, 3, rtrace::Material(0, 0, 255));

    scene.addObject(sphere);
	scene.addObject(torus);
    scene.addObject(std::make_shared<Sphere>(rtrace::Vector3(0, 25, 25), 7, rtrace::Material(0, 255, 0)));
    scene.addObject(std::make_shared<Plane>(rtrace::Vector3(0, 0, 0), rtrace::Vector3::UnitZ(), rtrace::Material(64, 64, 64)));

    scene.addLight(std::make_shared<rtrace::Light>(rtrace::Vector3(-25, 0, 25)));
	scene.addLight(std::make_shared<rtrace::Light>(rtrace::Vector3(0, 0, 50)));

    double angle = 0;

    while(scene.isOpen()) {
        handleEvents(scene, camera);

        camera.rotate(scene);
        camera.move();

        /*-----------  Scene objects update  ---------------*/

		{
			rtrace::Transform3 tr = sphere->getTransform();

			tr.translate(rtrace::Vector3(0, 2*std::cos(angle), 0));
			angle +=0.1;

			sphere->setTransform(tr);
		}

		{
			rtrace::Transform3 tr = torus->getTransform();

			tr.rotate(rtrace::Vector3(0, 1.1, 0.75), 0.1);
			
			torus->setTransform(tr);
		}

        /*-----------  Scene render  ---------------*/

        scene.display(camera);
    }

}
