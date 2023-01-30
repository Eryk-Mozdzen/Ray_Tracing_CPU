#include <rtrace/scene.h>
#include <iomanip>
#include <sstream>
#include <cmath>

namespace rtrace {

	Scene::Scene() {

	}

	void Scene::add(const Object &object) {
		objects.push_back(object);
	}

	void Scene::add(const Light &light) {
		lights.push_back(light);
	}

	Collision Scene::traceRay(const Ray &ray) const {
		Collision collision;

		for(const Object &object : objects) {

			const Collision curr = object.intersect(ray);

			if(curr.exist) {
				if(curr.distance>EPSILON && curr.distance<collision.distance) {
					collision = curr;
				}
			}
		}

		return collision;
	}

	Collision Scene::traceSphere(const Vector3 &point) const {
		Collision collision;

		for(const Object &object : objects) {

			const Collision curr = object.distance(point);

			collision = Collision::smin(curr, collision, 10);
		}

		return collision;
	}

	Color Scene::recursiveRayTracing(const Ray &ray, int depth) const {
		if(depth==0)
			return Color();

		const Collision data = traceRay(ray);

		if(!data.exist)
			return Color();

		const Vector3 N = normalize(data.normal);       // normal
		const Vector3 V = normalize(ray.direction);		// view
		const Vector3 H = normalize(V - 2*(V*N)*N);		// view reflection
		
		const Color reflected = recursiveRayTracing(Ray(data.point, H), depth - 1);

		Color illumination = data.material.ambient*data.material.color + data.material.reflection*reflected;

		for(const Light &light : lights) {
			const Vector3 L = normalize(light.getPosition() - data.point);	// light
			const Vector3 R = normalize(L - 2*(L*N)*N);						// light reflection

			const Collision shadow = traceRay(Ray(data.point, L));
			if(shadow.exist && shadow.distance<length(light.getPosition() - data.point))
				continue;

			illumination +=data.material.diffuse*std::max(L*N, 0.)*data.material.color;
			illumination +=data.material.specular*std::pow(std::max(V*R, 0.), data.material.shininess)*Color::white;
		}

		return illumination;
	}

	std::vector<Color> Scene::renderRayTracing(const View &view, int width, int height, int depth) const {

		std::vector<Color> buffer(width*height);

		const Vector3 dirX = view.getDirection(Vector3::X)*view.getDistanceFromProjectionPlane();
		const Vector3 dirY = view.getDirection(Vector3::Y)/width*width/height;
		const Vector3 dirZ = view.getDirection(Vector3::Z)/height;

		Ray ray(view.getPosition(), Vector3::X);

		for(int i=0; i<height; i++) {
			for(int j=0; j<width; j++) {
				
				ray.direction = normalize(dirX + dirY*(width/2. - j) + dirZ*(height/2. - i));

				buffer[i*width + j] = recursiveRayTracing(ray, depth);
			}
		}

		return buffer;
	}

	std::vector<Color> Scene::renderSphereTracing(const View &view, int width, int height) const {

		std::vector<Color> buffer(width*height);

		const Vector3 dirX = view.getDirection(Vector3::X)*view.getDistanceFromProjectionPlane();
		const Vector3 dirY = view.getDirection(Vector3::Y)/width*width/height;
		const Vector3 dirZ = view.getDirection(Vector3::Z)/height;

		Ray ray(view.getPosition(), Vector3::X);
		Collision collision;
		
		constexpr double renderView = 100;
		double dist;

		for(int i=0; i<height; i++) {
			for(int j=0; j<width; j++) {
				
				collision.exist = false;
				ray.direction = normalize(dirX + dirY*(width/2. - j) + dirZ*(height/2. - i));
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

}