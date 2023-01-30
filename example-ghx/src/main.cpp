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
#include <cmath>

int main() {

    Window window(135, 90, 3);
	Camera camera(rtrace::Vector3(-50, 0, 15), window);

    /*-----------  Scene setup  ---------------*/

	Sphere sphere1(rtrace::Vector3(10, 0, 30), 7, rtrace::Color::red);
	Sphere sphere2(rtrace::Vector3(0, 25, 25), 7, rtrace::Color::green);
	Plane plane(rtrace::Vector3(0, 0, 0), rtrace::Vector3::Z);
	Torus torus(rtrace::Vector3(0, 0, 15), 6, 3);

    window.add(sphere1);
	window.add(sphere2);
	window.add(torus);
    window.add(plane);

	rtrace::Light light1(rtrace::Vector3(-25, 0, 25));
	rtrace::Light light2(rtrace::Vector3(0, 0, 50));

    window.add(light1);
	window.add(light2);

    double angle = 0;

    while(window.isOpen()) {
        window.handleEvents();

        camera.move();

        /*-----------  Scene objects update  ---------------*/

		sphere1.transform.translate(rtrace::Vector3(0, 2*std::cos(angle), 0));
		angle +=0.1;

		torus.transform.rotate(rtrace::Vector3(0, 1.1, 0.75), 0.1);

        /*-----------  Scene render  ---------------*/

        window.display(camera);
    }

}
