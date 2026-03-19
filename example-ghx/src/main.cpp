/*  Ray tracing example program
    by Eryk Możdżeń, May 2021r.

    Simple usage of implemented features like:
        - definition of user custom class
        - ray tracing rendering for objects with intersect() method
        - sphere tracing rendering for objects with distance() method

    Controls:
        - Mouse         - camera rotation
        - Mouse Scroll  - resolution change
        - WASD          - camera forward, left, backward, right movement
        - LShift, LCtr  - camera up, down movement
        - N/M keys      - toggling between two modes (ray/sphere tracing)
        - Z key         - take a screenshot
        - X key         - exit the application		*/

#include <chrono>
#include <cmath>

#include "Camera.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Torus.hpp"
#include "Window.hpp"

int main() {
    Window window(135, 90, 3);
    Camera camera({-50, 0, 15}, window);

    Sphere sphere1({10, 0, 30}, 7, rtrace::Color::red);
    Sphere sphere2({0, 25, 25}, 7, rtrace::Color::green);
    Plane plane({0, 0, 0}, rtrace::Vector3::Z);
    Torus torus({0, 0, 15}, 6, 3);

    window.add(sphere1);
    window.add(sphere2);
    window.add(torus);
    window.add(plane);

    rtrace::Light light1({-25, 0, 25});
    rtrace::Light light2({0, 0, 50});

    window.add(light1);
    window.add(light2);

    while(window.isOpen()) {
        window.handleEvents();

        camera.move();

        const double t = std::chrono::duration_cast<std::chrono::duration<double>>(
                             std::chrono::steady_clock::now().time_since_epoch())
                             .count();

        sphere1.transform.setTranslation({10, 20 * std::cos(2 * t), 30});

        torus.transform.setRotation({0, 1.1, 0.75}, t);

        window.display(camera);
    }
}
