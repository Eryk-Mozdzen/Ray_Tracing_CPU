#include "RayTracing.h"

RenderScene::RenderScene() {
    this->setReflectionDepth(1);
}

RenderScene::RenderScene(const RenderMode &renderMode, 
                            const unsigned int &reflectionDepth, 
                            const unsigned int &resolutionWidgth, 
                            const unsigned int &resolutionHeight) : sf::RenderWindow(sf::VideoMode(1280, 720), "Ray Tracing") {
    this->setRenderMode(renderMode);
    this->setReflectionDepth(reflectionDepth);
    this->setRenderResolution(resolutionWidgth, resolutionHeight);
}

void RenderScene::clearObjects() {
    //for(unsigned int i=0; i<this->objects.size(); i++)
    //    delete this->objects[i];
    this->objects.clear();
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
    this->renderResolution = sf::Vector2u(resolutionH, resolutionV);
}

Object* RenderScene::getObjectReference(const unsigned int &index) {
    assert(index>=0 && index<this->objects.size());

    return this->objects[index];
}

const unsigned int & RenderScene::getReflectionDepth() const {
    return this->reflectionDepth;
}

const sf::Vector2u & RenderScene::getRenderResolution() const {
    return this->renderResolution;
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
        data = CollisionData::smin(tmp, data, 10);
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
    const double w = this->renderResolution.x;
    const double h = this->renderResolution.y;
    const double aspectRatio = (double)this->renderResolution.x/this->renderResolution.y;

    sf::Image frameBuffer;
    frameBuffer.create(w, h, sf::Color::Black);

    for(unsigned int i=0; i<h; i++) {
        for(unsigned int j=0; j<w; j++) {
            Vector3 dir = normalize(
                view.getDistanceFromProjectionPlane()*directionX +
                (j-w/2.)/w*aspectRatio*directionY +
                (i-h/2.)/h*directionZ
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

void RenderScene::display(const View &view) {
    sf::Clock clock;
    clock.restart();
    sf::Image frameBuffer = this->render(view);
    const double renderTime = clock.restart().asSeconds();

    frameBuffer.flipVertically();
	frameBuffer.flipHorizontally();

    sf::Texture tex;
    tex.loadFromImage(frameBuffer);

    sf::Sprite sprite;
    sprite.setTexture(tex);
    sprite.scale((double)this->getSize().x/frameBuffer.getSize().x, (double)this->getSize().y/frameBuffer.getSize().y);

    std::stringstream windowTitle;
    windowTitle << std::setprecision(5) << std::fixed;
    windowTitle << "Ray Tracing";
    windowTitle << " | Resolution: " << this->renderResolution.x << "x" << this->renderResolution.y;
    windowTitle << " | Render Time: " << renderTime << "s";
    windowTitle << " | FPS: " << 1/renderTime;
    windowTitle << " | Zoom: " << view.getDistanceFromProjectionPlane();
    sf::RenderWindow::setTitle(windowTitle.str());
    
    sf::RenderWindow::draw(sprite);
    sf::RenderWindow::display();
}


