#include "../include/RayTracing.h"

Camera::Camera() {}

Camera::Camera(const Vector3 &position, const double &distance) {
    this->distance = distance;

    this->transform.translate(position);

    this->rays.resize(RESOLUTION_H*RESOLUTION_V);

    this->update_rays();
}

void Camera::update_rays() {
    Vector3 directionX = this->getDirectionX();
    Vector3 directionY = this->getDirectionY();
    Vector3 directionZ = this->getDirectionZ();

    for(int i=0; i<RESOLUTION_V; i++) {
        for(int j=0; j<RESOLUTION_H; j++) {
            Vector3 dir = normalize(
                distance*directionX +
                (j-RESOLUTION_H/2.)/RESOLUTION_H*ASPECT_RATIO*directionY +
                (i-RESOLUTION_V/2.)/RESOLUTION_V*directionZ
            );

            rays[i*RESOLUTION_H + j] = Ray(this->transform.getTranslation(), dir);
        }
    }
}

Vector3 Camera::getDirectionX() {
    return normalize(this->transform.getRelativeToReferenceFrame(Vector3::UnitX()) - this->transform.getTranslation());
}

Vector3 Camera::getDirectionY() {
    return normalize(this->transform.getRelativeToReferenceFrame(Vector3::UnitY()) - this->transform.getTranslation());
}

Vector3 Camera::getDirectionZ() {
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
        if(objects[i]->intersect(ray, &tmp)) {
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

    if(exist) {
        //sf::Color color_reflected = trace(Ray(point, ray.direction - 2.f*dot(ray.direction, normal)*normal), depth-1);
        sf::Color color_reflected = (data.reflectivity>EPSILON) ?
            trace(Ray(data.point, ray.direction - 2.f*(ray.direction*data.normal)*data.normal), depth-1) :
            sf::Color::Transparent;

        //double brightness = dot(light_ray.direction, normal);

        //return color_hit;
        //return color_reflected;
        //return sf::Color(brightness, brightness, brightness);
        //return (shadow || brightness<0) ? sf::Color::Black : sf::Color::White;

        return color_interpolation(data.color, color_reflected, data.reflectivity);
    }

    return sf::Color::Transparent;
}

void Scene::render(const Camera &camera, sf::RenderWindow &window) const {
    const double pixel_w = (double)window.getSize().x/RESOLUTION_H;
    const double pixel_h = (double)window.getSize().y/RESOLUTION_V;

    sf::RectangleShape pixel(sf::Vector2f(pixel_w, pixel_h));

    for(int i=0; i<RESOLUTION_V; i++) {
        for(int j=0; j<RESOLUTION_H; j++) {
            sf::Color color = trace(camera.rays[i*RESOLUTION_H + j], REFLECTION_DEPTH);

            if(color!=sf::Color::Transparent && color!=sf::Color::Black) {
                pixel.setFillColor(color);
                pixel.setPosition(sf::Vector2f(j*pixel_w, i*pixel_h));
                //pixel.setPosition(sf::Vector2f(j*pixel_w, (RESOLUTION_V - i)*pixel_h));

                window.draw(pixel);
            }
        }
    }

}




