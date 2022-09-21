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

#include "window.h"
#include "camera.h"
#include "sphere.h"
#include "plane.h"
#include "torus.h"

int main() {

    Camera camera(rtrace::Vector3(-50, 0, 15), 1);
    Window window(rtrace::Scene::RAY_TRACING, 3, 135, 90);

    /*-----------  Scene setup  ---------------*/

	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(rtrace::Vector3(10, 0, 30), 7, rtrace::Color::red);
	std::shared_ptr<Torus> torus = std::make_shared<Torus>(rtrace::Vector3(0, 0, 15), 6, 3);

    window.addObject(sphere);
	window.addObject(torus);
    window.addObject(std::make_shared<Sphere>(rtrace::Vector3(0, 25, 25), 7, rtrace::Color::green));
    window.addObject(std::make_shared<Plane>(rtrace::Vector3(0, 0, 0), rtrace::Vector3::Z));

    window.addLight(std::make_shared<rtrace::Light>(rtrace::Vector3(-25, 0, 25)));
	window.addLight(std::make_shared<rtrace::Light>(rtrace::Vector3(0, 0, 50)));

    double angle = 0;

    while(window.isOpen()) {
        window.handleEvents();

        camera.rotate(window);
        camera.move();

        /*-----------  Scene objects update  ---------------*/

		sphere->transform.translate(rtrace::Vector3(0, 2*std::cos(angle), 0));
		angle +=0.1;

		torus->transform.rotate(rtrace::Vector3(0, 1.1, 0.75), 0.1);

        /*-----------  Scene render  ---------------*/

        window.display(camera);
    }

}
