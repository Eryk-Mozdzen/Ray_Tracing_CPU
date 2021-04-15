#include "../include/RayTracing.h"

View::View() {
    this->distance = 1;
}

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

Vector3 View::getPosition() const {
    return this->transform.getTranslation();
}

const Transform3 & View::getTransform() const {
    return this->transform;
}

double View::getDistanceFromProjectionPlane() const {
    return this->distance;
}

void View::setDistanceFromProjectionPlane(const double &distance) {
    this->distance = distance;
}

void View::translate(const Vector3 &transltion) {
    this->transform.translate(transltion);
}

void View::rotate(const Vector3 &axis, const double &theta) {
    this->transform.rotate(axis, theta);
}

Scene::Scene() {}

void Scene::addObject(Object *object_ptr) {
    this->objects.push_back(object_ptr);
}

void Scene::addLightSource(LightSource *light) {
    this->lights.push_back(light);
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

    //Vector3 nextDir = ray.direction - 2.f*(ray.direction*data.normal)*data.normal;
    //sf::Color color_reflected = trace(Ray(data.point, nextDir), depth-1);
    //return color_interpolation(data.color, color_reflected, 0.3);

    const double ka = data.material.getAmbient();
    const double kd = data.material.getDiffuse();
    const double ks = data.material.getSpecular();
    const double a = data.material.getShininess();
    const Vector3 N = normalize(data.normal);
    const Vector3 V = normalize(ray.point - data.point);

    double illumination = data.material.getAmbient()*255;
    for(int i=0; i<this->lights.size(); i++) {
        const Vector3 L = normalize(lights[i]->getPosition() - data.point);
        const Vector3 R = normalize(2*(L*N)*N - L);

        illumination +=kd*std::max(L*N, 0.)*255;
        illumination +=ks*std::pow(std::max(V*R, 0.), a)*255;
    }

    return sf::Color(illumination, illumination, illumination);
}

sf::Image Scene::render(const View &view, const int &resolutionH, const int &resolutionV) const {
    const Vector3 directionX = view.getDirectionX();
    const Vector3 directionY = view.getDirectionY();
    const Vector3 directionZ = view.getDirectionZ();
    const double aspectRatio = (double)resolutionH/resolutionV;

    sf::Image frame;
    frame.create(resolutionH, resolutionV, sf::Color::Black);

    for(int i=0; i<resolutionV; i++) {
        for(int j=0; j<resolutionH; j++) {
            Vector3 dir = normalize(
                view.getDistanceFromProjectionPlane()*directionX +
                (j-resolutionH/2.)/resolutionH*aspectRatio*directionY +
                (i-resolutionV/2.)/resolutionV*directionZ
            );

            sf::Color color = trace(Ray(view.getPosition(), dir), REFLECTION_DEPTH);

            if(color!=sf::Color::Transparent && color!=sf::Color::Black)
                frame.setPixel(j, i, color);
        }
    }

    return frame;
}




