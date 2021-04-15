#include "../include/RayTracing.h"

View::View() {}

View::View(const Vector3 &position, const double &distance) {
    this->distance = distance;

    this->transform.translate(position);
}

Vector3 View::getDirectionX() const {
    return normalize(this->transform.getRelativeToReferenceFrame(Vector3::UnitX()) - this->transform.getTranslation());
}

Vector3 View::getDirectionY() const {
    return normalize(this->transform.getRelativeToReferenceFrame(Vector3::UnitY()) - this->transform.getTranslation());
}

Vector3 View::getDirectionZ() const {
    return normalize(this->transform.getRelativeToReferenceFrame(Vector3::UnitZ()) - this->transform.getTranslation());
}

Scene::Scene() {}

void Scene::add(Object *object_ptr) {
    objects.push_back(object_ptr);
}

void Scene::clear() {
    objects.clear();
}

sf::Color Scene::trace(const Ray &ray, const int &depth) const {
    if(depth<=0)
        return sf::Color::Transparent;

    CollisionData tmp, data;
    double d, dist = 100000;
    bool exist = false;

    for(int i=0; i<objects.size(); i++) {
        if(objects[i]->intersect(ray, tmp)) {
            if(length(tmp.point-ray.point)>EPSILON) {
                d = length(tmp.point-ray.point);
                if(d<dist) {
                    dist = d;
                    data = tmp;
                }
                exist = true;
            }
        }
    }

    if(!exist)
        return sf::Color::Transparent;

    Vector3 nextDir = ray.direction - 2.f*(ray.direction*data.normal)*data.normal;

    sf::Color color_reflected = trace(Ray(data.point, nextDir), depth-1);

    return color_interpolation(data.color, color_reflected, 1);
}

sf::Image Scene::render(const View &view, const int &resolutionH, const int &resolutionV) const {
    const Vector3 directionX = view.getDirectionX();
    const Vector3 directionY = view.getDirectionY();
    const Vector3 directionZ = view.getDirectionZ();

    sf::Image frame;
    frame.create(resolutionH, resolutionV, sf::Color::Black);

    for(int i=0; i<resolutionV; i++) {
        for(int j=0; j<resolutionH; j++) {
            Vector3 dir = normalize(
                view.distance*directionX +
                (j-resolutionH/2.)/resolutionH*ASPECT_RATIO*directionY +
                (i-resolutionV/2.)/resolutionV*directionZ
            );

            Ray ray(view.transform.getTranslation(), dir);

            sf::Color color = trace(ray, REFLECTION_DEPTH);

            if(color!=sf::Color::Transparent && color!=sf::Color::Black) {
                frame.setPixel(j, i, color);
            }
        }
    }
    return frame;
}




