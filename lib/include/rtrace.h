#pragma once

#include <iomanip>
#include <vector>
#include <sstream>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "vector.h"
#include "matrix.h"
#include "tools.h"
#include "view.h"
#include "utils.h"
#include "material.h"
#include "transform.h"

/*  RenderMode enumerator
    Contains all possible modes for rendering.  */

namespace rtrace {

	enum RenderMode {
		RAY_TRACING_MODE,
		SPHERE_TRACING_MODE
	};

	/*  RenderWindow class
		In this place heppend all magic.
		It reprezent application window and all informations about scene with objects and light.*/

	class RenderScene : public sf::RenderWindow {
	private:
		std::vector<std::shared_ptr<Object>> objects;
		std::vector<std::shared_ptr<LightSource>> lights;
		sf::Vector2u renderResolution;
		sf::Image frameBuffer;
		RenderMode renderMode;
		unsigned int reflectionDepth;

		sf::Color renderPixel(const View&, const unsigned int&, const unsigned int&) const;

		CollisionData rayTrace(const Ray&) const;
		CollisionData sphereTrace(const Vector3&) const;
		sf::Color evaluateRayTracing(const Ray&, const unsigned int&) const;
		sf::Color evaluateSphereTracing(const Ray&, const unsigned int&) const;
	public:
		RenderScene(const RenderMode&, const unsigned int&, const unsigned int&, const unsigned int&);
		
		void addObject(std::shared_ptr<Object>);
		void addLight(std::shared_ptr<LightSource>);
		void setReflectionDepth(const unsigned int&);
		void setRenderMode(const RenderMode&);
		void setRenderResolution(const unsigned int&, const unsigned int&);

		const unsigned int & getReflectionDepth() const;
		const sf::Vector2u & getRenderResolution() const;

		const sf::Image & render(const View&);
		void display(const View&);
		
		void saveFrameToFile(const std::string&) const;
	};

}
