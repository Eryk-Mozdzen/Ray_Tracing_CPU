/*  Ray tracing example program 
    by Eryk Możdżeń, September 2022r.

    Simple usage of implemented features like:
        - definition of user custom class
        - ray tracing rendering for objects with intersect() method		*/

#include "window.h"
#include "sphere.h"
#include "plane.h"
#include "torus.h"

int main() {

	Window window(200, 50);
    const rtrace::View view({-30, 0, 20}, 1);

    /*-----------  Scene setup  ---------------*/

	std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(rtrace::Vector3(10, 0, 30), 7);
	std::shared_ptr<Torus> torus = std::make_shared<Torus>(rtrace::Vector3(0, 0, 15), 6, 3);

    window.addObject(sphere);
	window.addObject(torus);
    window.addObject(std::make_shared<Sphere>(rtrace::Vector3(0, 25, 25), 7));
    window.addObject(std::make_shared<Plane>(rtrace::Vector3(0, 0, 0), rtrace::Vector3::Z));

    window.addLight(std::make_shared<rtrace::Light>(rtrace::Vector3(-25, 0, 25)));
	window.addLight(std::make_shared<rtrace::Light>(rtrace::Vector3(0, 0, 50)));

    double angle = 0;

    while(true) {

        /*-----------  Scene objects update  ---------------*/

		sphere->transform.translate({0, 2*std::cos(angle), 0});
		angle +=0.1;

		torus->transform.rotate({0, 1.1, 0.75}, 0.1);

        /*-----------  Scene render  ---------------*/

        window.display(view);
    }

}
