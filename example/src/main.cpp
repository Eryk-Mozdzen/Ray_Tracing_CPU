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

    Camera camera(rtrace::Vector3(-50, 0, 25), 1);
    Window window(rtrace::Scene::RAY_TRACING, 1, 135, 90);

    /*-----------  Scene setup  ---------------*/

	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(rtrace::Vector3(10, 0, 30), 7, rtrace::Material(255, 0, 0));
	std::shared_ptr<Torus> torus = std::make_shared<Torus>(rtrace::Vector3(0, 0, 15), 6, 3, rtrace::Material(0, 0, 255));

    window.addObject(sphere);
	window.addObject(torus);
    window.addObject(std::make_shared<Sphere>(rtrace::Vector3(0, 25, 25), 7, rtrace::Material(0, 255, 0)));
    window.addObject(std::make_shared<Plane>(rtrace::Vector3(0, 0, 0), rtrace::Vector3::UnitZ(), rtrace::Material(64, 64, 64)));

    window.addLight(std::make_shared<rtrace::Light>(rtrace::Vector3(-25, 0, 25)));
	window.addLight(std::make_shared<rtrace::Light>(rtrace::Vector3(0, 0, 50)));

    double angle = 0;
	rtrace::Transform3 tr;

    while(window.isOpen()) {
        window.handleEvents();

        camera.rotate(window);
        camera.move();

        /*-----------  Scene objects update  ---------------*/

		tr = sphere->getTransform();
		tr.translate(rtrace::Vector3(0, 2*std::cos(angle), 0));
		sphere->setTransform(tr);
		angle +=0.1;

		tr = torus->getTransform();
		tr.rotate(rtrace::Vector3(0, 1.1, 0.75), 0.1);
		torus->setTransform(tr);

        /*-----------  Scene render  ---------------*/

        window.display(camera);
    }

}
