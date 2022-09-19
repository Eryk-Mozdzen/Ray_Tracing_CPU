#include "rtrace.h"

rtrace::Scene::Scene(const rtrace::RenderMode &renderMode, 
                            const unsigned int &reflectionDepth, 
                            const unsigned int &resolutionWidgth, 
                            const unsigned int &resolutionHeight) : 
							sf::RenderWindow{sf::VideoMode(1280, 720), "Render Scene"},
							renderMode{renderMode},
							reflectionDepth{reflectionDepth} {

    this->setRenderResolution(resolutionWidgth, resolutionHeight);
}

void rtrace::Scene::addObject(std::shared_ptr<Object> object_ptr) {
    this->objects.push_back(std::move(object_ptr));
}

void rtrace::Scene::addLight(std::shared_ptr<Light> light) {
    this->lights.push_back(std::move(light));
}

void rtrace::Scene::setReflectionDepth(const unsigned int &reflectionDepth) {
    this->reflectionDepth = reflectionDepth;
}

void rtrace::Scene::setRenderMode(const rtrace::RenderMode &renderMode) {
    this->renderMode = renderMode;
}

void rtrace::Scene::setRenderResolution(const unsigned int &resolutionH, const unsigned int &resolutionV) {
    this->renderResolution = sf::Vector2u(resolutionH, resolutionV);

    this->frameBuffer.create(resolutionH, resolutionV, sf::Color::Black);
}

const unsigned int & rtrace::Scene::getReflectionDepth() const {
    return this->reflectionDepth;
}

const sf::Vector2u & rtrace::Scene::getRenderResolution() const {
    return this->renderResolution;
}

rtrace::CollisionData rtrace::Scene::rayTrace(const rtrace::Ray &ray) const {
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

rtrace::CollisionData rtrace::Scene::sphereTrace(const rtrace::Vector3 &point) const {
    CollisionData tmp, data;

    for(unsigned int i=0; i<this->objects.size(); i++) {
        tmp = this->objects[i]->distance(point);

        //data = CollisionData::min(tmp, data);
        data = CollisionData::smin(tmp, data, 10);
    }

    return data;
}

sf::Color rtrace::Scene::evaluateRayTracing(const rtrace::Ray &ray, const unsigned int &depth) const {
    if(depth>=this->reflectionDepth)
        return sf::Color::Transparent;

    const CollisionData data = this->rayTrace(ray);

    if(!data.exist)
        return sf::Color::Transparent;

    const Vector3 N = normalize(data.normal);       // normal
	const Vector3 V = normalize(ray.direction);		// view
    const Vector3 H = normalize(V - 2*(V*N)*N);		// view reflection
	
	const sf::Color reflected = this->evaluateRayTracing(Ray(data.point, H), depth + 1);

    sf::Color illumination = data.material.getAmbient()*data.material.getColor() + data.material.getReflection()*reflected;

	for(const std::shared_ptr<Light> &light : this->lights) {
        const Vector3 L = normalize(light->getPosition() - data.point);   // light
        const Vector3 R = normalize(L - 2*(L*N)*N);                       // light reflection

        const CollisionData shadow = this->rayTrace(Ray(data.point, L));
		if(shadow.exist && shadow.distance<length(light->getPosition() - data.point))
			continue;

        illumination +=data.material.getDiffuse()*std::max(L*N, 0.)*data.material.getColor();
        illumination +=data.material.getSpecular()*std::pow(std::max(V*R, 0.), data.material.getShininess())*sf::Color::White;
    }

    return illumination;
}

sf::Color rtrace::Scene::evaluateSphereTracing(const rtrace::Ray &ray, const unsigned int &depth) const {
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

    return data.material.getColor();
}

sf::Color rtrace::Scene::renderPixel(const rtrace::View &view, const unsigned int &x, const unsigned int &y) const {
	const double w = this->renderResolution.x;
    const double h = this->renderResolution.y;
    const double aspectRatio = (double)this->renderResolution.x/this->renderResolution.y;

	const rtrace::Vector3 dir = normalize(
		view.getDistanceFromProjectionPlane()*view.getDirectionX() +
		(x-w/2.)/w*aspectRatio*view.getDirectionY() +
		(y-h/2.)/h*view.getDirectionZ()
	);

	sf::Color color;

	switch(this->renderMode) {
		case RAY_TRACING_MODE:      color = this->evaluateRayTracing(rtrace::Ray(view.getPosition(), dir), 0); break;
		case SPHERE_TRACING_MODE:   color = this->evaluateSphereTracing(rtrace::Ray(view.getPosition(), dir), 0); break;
		default: 					color = sf::Color::Transparent; break;
	}
	
	if(color==sf::Color::Transparent)
		color = sf::Color::Black;

	return color;
}

const sf::Image & rtrace::Scene::render(const rtrace::View &view) {

    for(unsigned int i=0; i<this->renderResolution.y; i++) {
        for(unsigned int j=0; j<this->renderResolution.x; j++) {

			sf::Color color = this->renderPixel(view, j, i);

			if(color==sf::Color::Transparent)
				color = sf::Color::Black;

			this->frameBuffer.setPixel(
				this->renderResolution.x - j - 1, 
				this->renderResolution.y - i - 1, 
				color
			);
        }
	}

    return this->frameBuffer;
}

void rtrace::Scene::display(const rtrace::View &view) {
    sf::Clock clock;
    clock.restart();
    this->render(view);
    const double renderTime = clock.restart().asSeconds();

    sf::Texture tex;
    tex.loadFromImage(this->frameBuffer);

    sf::Sprite sprite;
    sprite.setTexture(tex);
    sprite.scale((double)this->getSize().x/this->frameBuffer.getSize().x, (double)this->getSize().y/this->frameBuffer.getSize().y);
	sf::RenderWindow::draw(sprite);

    std::stringstream windowTitle;
    windowTitle << std::setprecision(3) << std::fixed;
    windowTitle << "Ray Tracing";
    windowTitle << " | Resolution: " << this->renderResolution.x << "x" << this->renderResolution.y;
    windowTitle << " | Render Time: " << renderTime*1000. << "ms";
    windowTitle << " | FPS: " << 1/renderTime;
    windowTitle << " | Zoom: " << view.getDistanceFromProjectionPlane();

    windowTitle << " | Mode: ";
    switch(this->renderMode) {
        case rtrace::RAY_TRACING_MODE:      windowTitle << "ray tracing"; break;
        case rtrace::SPHERE_TRACING_MODE:   windowTitle << "sphere tracing"; break;
        default: break;
    }

    sf::RenderWindow::setTitle(windowTitle.str());
    
    sf::RenderWindow::display();
}

void rtrace::Scene::saveFrameToFile(const std::string &filename) const {
    this->frameBuffer.saveToFile(filename);
}


