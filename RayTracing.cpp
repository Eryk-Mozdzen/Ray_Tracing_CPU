#include "RayTracing.h"

Camera::Camera() {}

Camera::Camera(Vector3 _position, Vector3 _directionX, double _distance) {
    position = _position;
    distance = _distance;
    directionX = normalize(_directionX);

    directionX = normalize(directionX);
    directionY = normalize(Vector3(0, 0, 1)^directionX);
    directionZ = normalize(directionY^directionX);

    update_rays();
}

void Camera::move(sf::Vector2i mouse) {
    Vector3 p = position;
    Vector3 dx = directionX;
    Vector3 dy = directionY;
    Vector3 dz = directionZ;

    Vector3 translation(0, 0, 0);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))         translation +=directionX;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))         translation -=directionX;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))         translation -=directionY;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))         translation +=directionY;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))    translation -=directionZ;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))  translation +=directionZ;
    position +=LINEAR_VELOCITY*normalize(translation);

    if(mouse.x<0) {
        directionX = rotate(directionZ, directionX, ANGULAR_VELOCITY);
        directionY = rotate(directionZ, directionY, ANGULAR_VELOCITY);
    } else if(mouse.x>0) {
        directionX = rotate(directionZ, directionX, -ANGULAR_VELOCITY);
        directionY = rotate(directionZ, directionY, -ANGULAR_VELOCITY);
    }

    if(mouse.y>0) {
        directionX = rotate(directionY, directionX, ANGULAR_VELOCITY);
        directionZ = rotate(directionY, directionZ, ANGULAR_VELOCITY);
    } else if(mouse.y<0) {
        directionX = rotate(directionY, directionX, -ANGULAR_VELOCITY);
        directionZ = rotate(directionY, directionZ, -ANGULAR_VELOCITY);
    }

    if((directionY*Vector3(0, 0, 1))<-0.1) {
        directionY = rotate(directionX, directionY, ANGULAR_VELOCITY);
        directionZ = rotate(directionX, directionZ, ANGULAR_VELOCITY);
    } else if((directionY*Vector3(0, 0, 1))>0.1) {
        directionY = rotate(directionX, directionY, -ANGULAR_VELOCITY);
        directionZ = rotate(directionX, directionZ, -ANGULAR_VELOCITY);
    }

    if(position!=p || directionX!=dx || directionY!=dy || directionZ!=dz) {
        update_rays();
    }

}

void Camera::update_rays() {
    directionX = normalize(directionX);
    directionY = normalize(directionY);
    directionZ = normalize(directionZ);

    screen = Plane(position + distance*directionX, directionX);
    rays.resize(RESOLUTION_H*RESOLUTION_V);

    for(int i=0; i<RESOLUTION_V; i++) {
        for(int j=0; j<RESOLUTION_H; j++) {
            Vector3 dir = normalize(
                distance*directionX +
                (j-RESOLUTION_H/2.)/RESOLUTION_H*WINDOW_WIDTH*directionY +
                (i-RESOLUTION_V/2.)/RESOLUTION_V*WINDOW_HEIGHT*directionZ
            );

            rays[i*RESOLUTION_H + j] = Ray(position, dir);
        }
    }
}

Scene::Scene() {}

Scene::Scene(Camera *_camera) {
    camera = _camera;
}

sf::Color Scene::trace(Ray ray, int depth) {
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

void Scene::add(Object *object_ptr) {
    objects.push_back(object_ptr);
}

void Scene::clear() {
    objects.clear();
}

void Scene::render(sf::RenderWindow &window) {
    const double pixel_w = (double)window.getSize().x/RESOLUTION_H;
    const double pixel_h = (double)window.getSize().y/RESOLUTION_V;

    sf::RectangleShape pixel(sf::Vector2f(pixel_w, pixel_h));

    for(int i=0; i<RESOLUTION_V; i++) {
        for(int j=0; j<RESOLUTION_H; j++) {
            sf::Color color = trace(camera->rays[i*RESOLUTION_H + j], REFLECTION_DEPTH);

            if(color!=sf::Color::Transparent && color!=sf::Color::Black) {
                pixel.setFillColor(color);
                pixel.setPosition(sf::Vector2f(j*pixel_w, i*pixel_h));

                window.draw(pixel);
            }
        }
    }

}




