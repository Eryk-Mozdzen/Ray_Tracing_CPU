#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Vector.h"
#include "Matrix.h"

#include "RayTracingMath.h"
#include "RayTracingUtilities.h"
#include "RayTracingMaterial.h"
#include "RayTracingTransform.h"

class View {
private:
    double distance;
    Transform3 transform;
public:
    View();
    View(const Vector3&, const double&);

    Vector3 getDirectionX() const;
    Vector3 getDirectionY() const;
    Vector3 getDirectionZ() const;
    Vector3 getPosition() const;
    const Transform3 & getTransform() const;
    double getDistanceFromProjectionPlane() const;

    void setDistanceFromProjectionPlane(const double&);
    void translate(const Vector3&);
    void rotate(const Vector3&, const double&);
};

enum RenderMode {
    RAY_TRACING_MODE,
    SPHERE_TRACING_MODE
};

class RenderScene {
private:
    std::vector<Object*> objects;
    std::vector<LightSource*> lights;
    unsigned int reflectionDepth;
    unsigned int resolutionH, resolutionV;
    RenderMode renderMode;

    CollisionData rayTrace(const Ray&) const;
    CollisionData sphereTrace(const Vector3&) const;
    sf::Color evaluateRayTracing(const Ray&, const unsigned int&) const;
    sf::Color evaluateSphereTracing(const Ray&, const unsigned int&) const;
public:
    RenderScene();

    void addObject(Object*);
    void addLightSource(LightSource*);
    void setReflectionDepth(const unsigned int&);
    void setRenderMode(const RenderMode&);
    void setRenderResolution(const unsigned int&, const unsigned int&);

    Object* getObjectReference(const unsigned int&);
    const unsigned int & getReflectionDepth() const;
    const unsigned int & getRenderResolutionWidth() const;
    const unsigned int & getRenderResolutionHeight() const;

    sf::Image render(const View&) const;
};

#endif
