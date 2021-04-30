#include "RayTracing.h"

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

RenderScene::RenderScene() {
    this->setReflectionDepth(1);
}

void RenderScene::addObject(Object *object_ptr) {
    this->objects.push_back(object_ptr);
}

void RenderScene::addLightSource(LightSource *light) {
    this->lights.push_back(light);
}

void RenderScene::setReflectionDepth(const unsigned int &reflectionDepth) {
    this->reflectionDepth = reflectionDepth;
}

void RenderScene::setRenderMode(const RenderMode &renderMode) {
    this->renderMode = renderMode;
}

void RenderScene::setRenderResolution(const unsigned int &resolutionH, const unsigned int &resolutionV) {
    this->resolutionH = resolutionH;
    this->resolutionV = resolutionV;
}

Object* RenderScene::getObjectReference(const unsigned int &index) {
    assert(index>=0 && index<this->objects.size());

    return this->objects[index];
}

const unsigned int & RenderScene::getReflectionDepth() const {
    return this->reflectionDepth;
}

const unsigned int & RenderScene::getRenderResolutionWidth() const {
    return this->resolutionH;
}

const unsigned int & RenderScene::getRenderResolutionHeight() const {
    return this->resolutionV;
}

CollisionData RenderScene::rayTrace(const Ray &ray) const {
    CollisionData tmp, data;

    for(unsigned int i=0; i<this->objects.size(); i++) {
        tmp = this->objects[i]->intersect(ray);
        if(tmp.exist) {
            if(tmp.distance>EPSILON && tmp.distance<data.distance) {
                data = tmp;
            }
        }
    }

    return data;
}

CollisionData RenderScene::sphereTrace(const Vector3 &point) const {
    CollisionData tmp, data;

    for(unsigned int i=0; i<this->objects.size(); i++) {
        tmp = this->objects[i]->distance(point);

        //data = CollisionData::min(tmp, data);
        data = CollisionData::smin(tmp, data, 8);
    }

    return data;
}

sf::Color RenderScene::evaluateRayTracing(const Ray &ray, const unsigned int &depth) const {
    if(depth>=this->reflectionDepth)
        return sf::Color::Transparent;

    CollisionData data = this->rayTrace(ray);

    if(!data.exist)
        return sf::Color::Transparent;

    const Vector3 N = normalize(data.normal);               //normal
    const Vector3 V = normalize(ray.origin - data.point);   //view
    const Vector3 H = normalize(2*(V*N)*N + V);             //reflected view from surface

    sf::Color illumination = data.material.getAmbient()*data.color;

    for(unsigned int i=0; i<this->lights.size(); i++) {
        const Vector3 L = normalize(this->lights[i]->getPosition() - data.point);   //light
        const Vector3 R = normalize(2*(L*N)*N - L);                                 //reflected light from surface

        sf::Color reflected = this->evaluateRayTracing(Ray(data.point, H), depth+1);
        illumination +=data.material.getReflection()*reflected;

        CollisionData shadow = this->rayTrace(Ray(data.point, L));
        if(shadow.exist) {
            const double distToLight = length(data.point - this->lights[i]->getPosition());
            const double distToCollision = length(data.point - shadow.point);
            if(distToCollision<distToLight)
                continue;
        }

        illumination +=data.material.getDiffuse()*std::max(L*N, 0.)*data.color;
        illumination +=data.material.getSpecular()*std::pow(std::max(V*R, 0.), data.material.getShininess())*sf::Color::White;
    }

    return illumination;
}

sf::Color RenderScene::evaluateSphereTracing(const Ray &ray, const unsigned int &depth) const {
    if(depth>=this->reflectionDepth)
        return sf::Color::Transparent;

    CollisionData data;
    const double renderView = 100;
    double dist = 0;

    while(!data.exist && dist<renderView) {
        data = this->sphereTrace(ray.origin + ray.direction*dist);
        dist +=data.distance;
    }

    if(!data.exist)
        return sf::Color::Transparent;;

    return data.color;
}

sf::Image RenderScene::render(const View &view) const {
    const Vector3 position = view.getPosition();
    const Vector3 directionX = view.getDirectionX();
    const Vector3 directionY = view.getDirectionY();
    const Vector3 directionZ = view.getDirectionZ();
    const double aspectRatio = (double)this->resolutionH/this->resolutionV;

    sf::Image frameBuffer;
    frameBuffer.create(this->resolutionH, this->resolutionV, sf::Color::Black);

    for(unsigned int i=0; i<this->resolutionV; i++) {
        for(unsigned int j=0; j<this->resolutionH; j++) {
            Vector3 dir = normalize(
                view.getDistanceFromProjectionPlane()*directionX +
                (j-resolutionH/2.)/this->resolutionH*aspectRatio*directionY +
                (i-resolutionV/2.)/this->resolutionV*directionZ
            );

            sf::Color color;
            switch(this->renderMode) {
                case RAY_TRACING_MODE:      color = this->evaluateRayTracing(Ray(position, dir), 0); break;
                case SPHERE_TRACING_MODE:   color = this->evaluateSphereTracing(Ray(position, dir), 0); break;
                default: color = sf::Color::Transparent; break;
            }

            if(color!=sf::Color::Transparent && color!=sf::Color::Black)
                frameBuffer.setPixel(j, i, color);
        }
    }

    return frameBuffer;
}




