#pragma once

#include <vector>
#include <memory>
#include <iomanip>
#include <sstream>

#include <rtrace/view.h>
#include <rtrace/object.h>
#include <rtrace/light.h>
#include <rtrace/color.h>

namespace rtrace {

	/*  RenderWindow class
		In this place heppend all magic.
		It reprezent application window and all informations about scene with objects and light.	*/

	class Scene {
	private:
		std::vector<std::shared_ptr<Object>> objects;
		std::vector<std::shared_ptr<Light>> lights;

		Collision traceRay(const Ray&) const;
		Collision traceSphere(const Vector3&) const;

		Color recursiveRayTracing(const Ray&, int) const;
	public:
		Scene();
		
		void addObject(std::shared_ptr<Object>);
		void addLight(std::shared_ptr<Light>);

		std::vector<Color> renderRayTracing(const View&, int, int, int) const;
		std::vector<Color> renderSphereTracing(const View&, int, int) const;
	};

}
