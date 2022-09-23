/*  Ray tracing example program 
    by Eryk Możdżeń, September 2022r.

    Simple usage of implemented features like:
        - definition of user custom class
        - ray tracing rendering for objects with intersect() method		*/

#include "window.h"
#include "torus.h"

int main() {

	Window window(60, 30);
    const rtrace::View view({-25, 0, 0}, 1);

	std::shared_ptr<Torus> torus = std::make_shared<Torus>(rtrace::Vector3(0, 0, 0), 6, 3);
	window.addObject(torus);

    window.addLight(std::make_shared<rtrace::Light>(rtrace::Vector3(-25, 0, 25)));

    while(true) {

		torus->transform.rotate({0, 1.1, 0.75}, 0.005);

        window.display(view);
    }

}
