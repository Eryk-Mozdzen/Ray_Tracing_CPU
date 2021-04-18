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

CollisionData Scene::trace(const Ray &ray) const {
    CollisionData tmp, data;

    for(int i=0; i<this->objects.size(); i++) {
        if(this->objects[i]->intersect(ray, tmp)) {
            if(length(tmp.point-ray.point)>EPSILON) {
                if(tmp.distance<data.distance) {
                    data = tmp;
                }
                data.exist = true;
            }
        }
    }

    return data;
}

sf::Color Scene::evaluate(const Ray &ray, const int &depth) const {
    if(depth<=0)
        return sf::Color::Transparent;

    CollisionData data = this->trace(ray);

    if(!data.exist)
        return sf::Color::Transparent;

    const Vector3 N = normalize(data.normal);
    const Vector3 V = normalize(ray.point - data.point);

    sf::Color illumination = data.material.getAmbient()*data.color;
    for(int i=0; i<this->lights.size(); i++) {
        const Vector3 L = normalize(this->lights[i]->getPosition() - data.point);
        const Vector3 R = normalize(2*(L*N)*N - L);

        CollisionData shadow = this->trace(Ray(data.point, L));
        const double distToLight = length(data.point - this->lights[i]->getPosition());
        const double distToCollision = length(data.point - shadow.point);
        if(shadow.exist && distToCollision<distToLight)
            continue;

        illumination +=data.material.getDiffuse()*std::max(L*N, 0.)*data.color;
        illumination +=data.material.getSpecular()*std::pow(std::max(V*R, 0.), data.material.getShininess())*sf::Color::White;

        //const Vector3 H = normalize(2*(V*N)*N + V);
        //sf::Color reflected = this->evaluate(Ray(data.point, H), depth-1);

        //if(reflected!=sf::Color::Transparent)
            //illumination +=data.material.getReflection()*reflected;
            //illumination = color_interpolation(illumination, reflected.color, data.material.getReflection());
    }

    return illumination;
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

            sf::Color color = this->evaluate(Ray(view.getPosition(), dir), REFLECTION_DEPTH);

            if(color!=sf::Color::Transparent && color!=sf::Color::Black)
                frame.setPixel(j, i, color);
        }
    }

    return frame;
}




