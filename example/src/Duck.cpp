#include "Duck.h"

Duck::Duck() {
    this->washes = Material(255, 255, 255);
    this->skin = Material(255, 168, 0);
    this->eye = Material(255, 0, 0);

    body.push_back(new Sphere(Vector3(0, 0, 0), 5, this->washes));
    //body.push_back(new Sphere(Vector3(10, 0, 0), 5, this->washes));
    body.push_back(new Sphere(Vector3(5, 3, -2), 5, this->washes));
    body.push_back(new Sphere(Vector3(5, -3, -2), 5, this->washes));

    body.push_back(new Sphere(Vector3(-5, 0, 14), 2, this->washes));
    body.push_back(new Sphere(Vector3(-6, -1.5, 15), 0.4, this->eye));
    body.push_back(new Sphere(Vector3(-6, 1.5, 15), 0.4, this->eye));
    //body.push_back(new Box(Vector3(-6, 1, 14), Vector3(3, 2, 1), this->skin));
}

void Duck::construct(RenderScene &scene) const {


    for(Object* obj : this->body)
        scene.addObject(obj);
}

