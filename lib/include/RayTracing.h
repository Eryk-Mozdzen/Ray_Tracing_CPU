#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <iomanip>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Vector.h"
#include "Matrix.h"

#include "RayTracingMath.h"
#include "RayTracingView.h"
#include "RayTracingUtilities.h"
#include "RayTracingMaterial.h"
#include "RayTracingTransform.h"

/*  RenderMode enumerator
    Contains all possible modes for rendering.  */

enum RenderMode {
    RAY_TRACING_MODE,
    SPHERE_TRACING_MODE
};

/*  RenderWindow class
    In this place heppend all magic.
    It reprezent application window and all informations about scene with objects and light.*/

class RenderScene : public sf::RenderWindow {
private:
    std::vector<Object*> objects;
    std::vector<LightSource*> lights;
    sf::Vector2u renderResolution;
    sf::Image frameBuffer;
    RenderMode renderMode;
    unsigned int reflectionDepth;

    CollisionData rayTrace(const Ray&) const;
    CollisionData sphereTrace(const Vector3&) const;
    sf::Color evaluateRayTracing(const Ray&, const unsigned int&) const;
    sf::Color evaluateSphereTracing(const Ray&, const unsigned int&) const;
public:
    RenderScene();
    RenderScene(const RenderMode&, const unsigned int&, const unsigned int&, const unsigned int&);
    
    void clearObjects();
    void addObject(Object*);
    void addLightSource(LightSource*);
    void setReflectionDepth(const unsigned int&);
    void setRenderMode(const RenderMode&);
    void setRenderResolution(const unsigned int&, const unsigned int&);

    Object* getObjectReference(const unsigned int&);
    const unsigned int & getReflectionDepth() const;
    const sf::Vector2u & getRenderResolution() const;

    const sf::Image & render(const View&);
    void display(const View&);
    
    void saveFrameToFile(const std::string&) const;
};

#endif
