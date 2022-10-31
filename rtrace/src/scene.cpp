#include <rtrace/scene.h>

rtrace::Scene::Scene() {

}

void rtrace::Scene::addObject(std::shared_ptr<Object> object) {
    objects.push_back(std::move(object));
}

void rtrace::Scene::addLight(std::shared_ptr<Light> light) {
    lights.push_back(std::move(light));
}

rtrace::Collision rtrace::Scene::traceRay(const rtrace::Ray &ray) const {
	rtrace::Collision collision;

	for(const std::shared_ptr<rtrace::Object> &object : objects) {

		const rtrace::Collision curr = object->intersect(ray);

		if(curr.exist) {
			if(curr.distance>rtrace::EPSILON && curr.distance<collision.distance) {
                collision = curr;
            }
		}
	}

    return collision;
}

rtrace::Collision rtrace::Scene::traceSphere(const rtrace::Vector3 &point) const {
    rtrace::Collision collision;

    for(const std::shared_ptr<rtrace::Object> &object : objects) {

        const rtrace::Collision curr = object->distance(point);

        collision = Collision::smin(curr, collision, 10);
    }

    return collision;
}

rtrace::Color rtrace::Scene::recursiveRayTracing(const rtrace::Ray &ray, int depth) const {
    if(depth==0)
        return rtrace::Color();

    const Collision data = traceRay(ray);

    if(!data.exist)
        return rtrace::Color();

    const rtrace::Vector3 N = rtrace::normalize(data.normal);       // normal
	const rtrace::Vector3 V = rtrace::normalize(ray.direction);		// view
    const rtrace::Vector3 H = rtrace::normalize(V - 2*(V*N)*N);		// view reflection
	
	const rtrace::Color reflected = recursiveRayTracing(Ray(data.point, H), depth - 1);

    rtrace::Color illumination = data.material.ambient*data.material.color + data.material.reflection*reflected;

	for(const std::shared_ptr<Light> &light : lights) {
        const rtrace::Vector3 L = rtrace::normalize(light->getPosition() - data.point);   // light
        const rtrace::Vector3 R = rtrace::normalize(L - 2*(L*N)*N);                       // light reflection

        const rtrace::Collision shadow = traceRay(Ray(data.point, L));
		if(shadow.exist && shadow.distance<length(light->getPosition() - data.point))
			continue;

        illumination +=data.material.diffuse*std::max(L*N, 0.)*data.material.color;
        illumination +=data.material.specular*std::pow(std::max(V*R, 0.), data.material.shininess)*rtrace::Color::white;
    }

    return illumination;
}

std::vector<rtrace::Color> rtrace::Scene::renderRayTracing(const rtrace::View &view, int width, int height, int depth) const {

	std::vector<rtrace::Color> buffer(width*height);

	const rtrace::Vector3 dirX = view.getDirection(rtrace::Vector3::X)*view.getDistanceFromProjectionPlane();
	const rtrace::Vector3 dirY = view.getDirection(rtrace::Vector3::Y)/width*width/height;
	const rtrace::Vector3 dirZ = view.getDirection(rtrace::Vector3::Z)/height;

	rtrace::Ray ray(view.getPosition(), rtrace::Vector3::X);

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
			
			ray.direction = rtrace::normalize(dirX + dirY*(width/2. - j) + dirZ*(height/2. - i));

			buffer[i*width + j] = recursiveRayTracing(ray, depth);
        }
	}

    return buffer;
}

std::vector<rtrace::Color> rtrace::Scene::renderSphereTracing(const rtrace::View &view, int width, int height) const {

	std::vector<rtrace::Color> buffer(width*height);

	const rtrace::Vector3 dirX = view.getDirection(rtrace::Vector3::X)*view.getDistanceFromProjectionPlane();
	const rtrace::Vector3 dirY = view.getDirection(rtrace::Vector3::Y)/width*width/height;
	const rtrace::Vector3 dirZ = view.getDirection(rtrace::Vector3::Z)/height;

	rtrace::Ray ray(view.getPosition(), rtrace::Vector3::X);
	rtrace::Collision collision;
	
	constexpr double renderView = 100;
	double dist;

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
			
			collision.exist = false;
			ray.direction = rtrace::normalize(dirX + dirY*(width/2. - j) + dirZ*(height/2. - i));
			dist = 0;

			while(!collision.exist && dist<renderView) {
				collision = traceSphere(ray.origin + ray.direction*dist);
				dist +=collision.distance;
			}

			if(collision.exist) {
				buffer[i*width + j] = collision.material.color;
			}
        }
	}

    return buffer;
}
