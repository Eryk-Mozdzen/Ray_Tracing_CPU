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
        - X key         - exit the application
    */

#include <iostream>     //for cout, endl
#include <ctime>        //for random seed, screenshot name

#include "RayTracing.h"

/*-----------  User custom objects  ---------------*/

#include "Camera.h"
#include "Sphere.h"
#include "Plane.h"
#include "Torus.h"

void handleEvents(RenderScene &scene, Camera &camera) {
    sf::Event event;
    while(scene.pollEvent(event)) {
        if(event.type==sf::Event::Closed)
            scene.close();
        if(event.type==sf::Event::KeyPressed) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                scene.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
                scene.setRenderMode(SPHERE_TRACING_MODE);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                scene.setRenderMode(RAY_TRACING_MODE);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                scene.saveFrameToFile("../screenshots/screenshot_" + std::to_string(time(nullptr)) + ".jpg");
                std::cout << "Saved frame" << std::endl;
            }
        }
        /*if(event.type==sf::Event::MouseWheelScrolled) {
            if(event.mouseWheelScroll.delta>0)  camera.zoomIn();
            else                                camera.zoomOut();
        }*/
        if(event.type==sf::Event::MouseWheelScrolled) {
            sf::Vector2u res = scene.getRenderResolution();
            if(event.mouseWheelScroll.delta>0)  scene.setRenderResolution(res.x + 15, res.y + 10);
            else                                scene.setRenderResolution(res.x - 15, res.y - 10);
        }
    }
}

int main() {
    std::srand(time(nullptr));      //for custom objects random objects colors, not needed for lib

    Camera camera(Vector3(-40, 20, 20), 1);
    RenderScene scene(RAY_TRACING_MODE, 3, 75, 60);
    TextureMenager menager;

    /*-----------  Scene setup  ---------------*/

    scene.setMouseCursorVisible(false);

    menager.load("../textures/notexture.jpg");

    scene.addObject(new Sphere(Vector3(10, -15, 30), 7, Material(255, 0, 0)));
    scene.addObject(new Sphere(Vector3(0, 20, 20), 7, Material(0, 255, 0)));
    scene.addObject(new Torus(Vector3(0, 0, 20), 10, 5, Material(0, 0, 255)));
    scene.addObject(new Plane(Vector3(0, 0, 0), Vector3::UnitZ(), Material(menager.getTextureReference(0), 5000, 5000)));

    scene.addLightSource(new LightSource(Vector3(0, 0, 40)));
    scene.addLightSource(new LightSource(Vector3(20, 20, 40)));

    double angle = 0;

    /*-----------  Main loop  ---------------*/

    while(scene.isOpen()) {
        handleEvents(scene, camera);

        camera.rotate(scene);
        camera.move();

        /*-----------  Scene objects update  ---------------*/

        Sphere *sphere = dynamic_cast<Sphere*>(scene.getObjectReference(0));
        Transform3 tr = sphere->getTransform();

        tr.translate(Vector3(0, 2*std::sin(angle), 0));

        sphere->setTransform(tr);
        angle +=0.1;

        /*-----------  Scene render  ---------------*/

        scene.display(camera);
    }

}
