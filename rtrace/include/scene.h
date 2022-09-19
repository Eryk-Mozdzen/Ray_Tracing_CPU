#pragma once

#include <vector>
#include <memory>
#include <iomanip>
#include <sstream>

#include "view.h"
#include "object.h"
#include "light.h"
#include "color.h"

namespace rtrace {

	/*  RenderWindow class
		In this place heppend all magic.
		It reprezent application window and all informations about scene with objects and light.	*/

	class Scene {
	private:
		std::vector<std::shared_ptr<Object>> objects;
		std::vector<std::shared_ptr<Light>> lights;

		Collision rayTrace(const Ray&) const;
		Collision sphereTrace(const Vector3&) const;
		Color evaluateRayTracing(const Ray&, const unsigned int&) const;
		Color evaluateSphereTracing(const Ray&, const unsigned int&) const;
	public:
		enum Mode {
			RAY_TRACING,
			SPHERE_TRACING
		};

		Scene();
		
		void addObject(std::shared_ptr<Object>);
		void addLight(std::shared_ptr<Light>);

		std::vector<Color> render(const View&, const unsigned int&, const unsigned int&, const Mode&, const unsigned int&) const;
	};

}