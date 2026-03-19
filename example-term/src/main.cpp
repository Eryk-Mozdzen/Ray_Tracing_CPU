/*  Ray tracing example program
    by Eryk Możdżeń, September 2022r.

    Simple usage of implemented features like:
        - definition of user custom class
        - ray tracing rendering for objects with intersect() method		*/

#include <chrono>

#include "Torus.hpp"
#include "Window.hpp"

int main() {
    const rtrace::View view({-25, 0, 0}, 1);
    const rtrace::Light light({-25, 0, 25});

    Window window(60, 30);

    Torus torus({0, 0, 0}, 6, 3);

    window.add(torus);
    window.add(light);

    while(true) {
        const double t = std::chrono::duration_cast<std::chrono::duration<double>>(
                             std::chrono::steady_clock::now().time_since_epoch())
                             .count();

        torus.transform.setRotation({0, 1.1, 0.75}, t);

        window.display(view);
    }
}
