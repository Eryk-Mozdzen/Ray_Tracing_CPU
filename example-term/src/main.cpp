/*  Ray tracing example program 
    by Eryk Możdżeń, September 2022r.

    Simple usage of implemented features like:
        - definition of user custom class
        - ray tracing rendering for objects with intersect() method		*/

#include "window.h"
#include "torus.h"

int main() {
	
	const rtrace::View view({-25, 0, 0}, 1);
	const rtrace::Light light(rtrace::Vector3(-25, 0, 25));

	Window window(60, 30);
    
	Torus torus(rtrace::Vector3(0, 0, 0), 6, 3);

	window.add(torus);
    window.add(light);

    while(true) {

		torus.transform.rotate({0, 1.1, 0.75}, 0.005);

        window.display(view);
    }
}
