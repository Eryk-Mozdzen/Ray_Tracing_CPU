#include "scene.h"

rtrace::Scene::Scene() {

}

void rtrace::Scene::addObject(std::shared_ptr<Object> object_ptr) {
    this->objects.push_back(std::move(object_ptr));
}

void rtrace::Scene::addLight(std::shared_ptr<Light> light) {
    this->lights.push_back(std::move(light));
}

rtrace::Collision rtrace::Scene::rayTrace(const rtrace::Ray &ray) const {
    Collision tmp, data;

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

rtrace::Collision rtrace::Scene::sphereTrace(const rtrace::Vector3 &point) const {
    Collision tmp, data;

    for(unsigned int i=0; i<this->objects.size(); i++) {
        tmp = this->objects[i]->distance(point);

        //data = CollisionData::min(tmp, data);
        data = Collision::smin(tmp, data, 10);
    }

    return data;
}

sf::Color rtrace::Scene::evaluateRayTracing(const rtrace::Ray &ray, const unsigned int &depth) const {
    if(depth==0)
        return sf::Color::Transparent;

    const Collision data = this->rayTrace(ray);

    if(!data.exist)
        return sf::Color::Transparent;

    const Vector3 N = normalize(data.normal);       // normal
	const Vector3 V = normalize(ray.direction);		// view
    const Vector3 H = normalize(V - 2*(V*N)*N);		// view reflection
	
	const sf::Color reflected = this->evaluateRayTracing(Ray(data.point, H), depth - 1);

    sf::Color illumination = data.material.ambient*data.material.color + data.material.reflection*reflected;

	for(const std::shared_ptr<Light> &light : this->lights) {
        const Vector3 L = normalize(light->getPosition() - data.point);   // light
        const Vector3 R = normalize(L - 2*(L*N)*N);                       // light reflection

        const Collision shadow = this->rayTrace(Ray(data.point, L));
		if(shadow.exist && shadow.distance<length(light->getPosition() - data.point))
			continue;

        illumination +=data.material.diffuse*std::max(L*N, 0.)*data.material.color;
        illumination +=data.material.specular*std::pow(std::max(V*R, 0.), data.material.shininess)*sf::Color::White;
    }

    return illumination;
}

sf::Color rtrace::Scene::evaluateSphereTracing(const rtrace::Ray &ray, const unsigned int &depth) const {
    if(depth==0)
        return sf::Color::Transparent;

    Collision data;
    const double renderView = 100;
    double dist = 0;

    while(!data.exist && dist<renderView) {
        data = this->sphereTrace(ray.origin + ray.direction*dist);
        dist +=data.distance;
    }

    if(!data.exist)
        return sf::Color::Transparent;;

    return data.material.color;
}

std::vector<sf::Color> rtrace::Scene::render(const rtrace::View &view, 
												const unsigned int &width, 
												const unsigned int &height, 
												const rtrace::Scene::Mode &mode, 
												const unsigned int &reflectionDepth) const {

	const double aspectRatio = (double)width/height;

	std::vector<sf::Color> buffer(width*height, sf::Color::Black);

    for(unsigned int i=0; i<height; i++) {
        for(unsigned int j=0; j<width; j++) {
			
			const rtrace::Vector3 dir = normalize(
				view.getDistanceFromProjectionPlane()*view.getDirectionX() +
				(j-width/2.)/width*aspectRatio*view.getDirectionY() +
				(i-height/2.)/height*view.getDirectionZ()
			);

			const rtrace::Ray ray(view.getPosition(), dir);

			sf::Color color;

			switch(mode) {
				case RAY_TRACING:      color = this->evaluateRayTracing(ray, reflectionDepth);		break;
				case SPHERE_TRACING:   color = this->evaluateSphereTracing(ray, reflectionDepth);	break;
			}

			if(color!=sf::Color::Transparent) {
				buffer[i*width + j] = color;
			}
        }
	}

    return buffer;
}
