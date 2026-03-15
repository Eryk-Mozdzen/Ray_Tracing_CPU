#ifndef RTRACE_SCENE_HPP
#define RTRACE_SCENE_HPP

#include <functional>
#include <vector>

#include "rtrace/Color.hpp"
#include "rtrace/Light.hpp"
#include "rtrace/Object.hpp"
#include "rtrace/View.hpp"

namespace rtrace {

/*  RenderWindow class
    In this place happened all magic.
    It represent application window and all informations about scene with objects and light.    */

class Scene {
    std::vector<std::reference_wrapper<const Object>> objects;
    std::vector<std::reference_wrapper<const Light>> lights;

    Collision traceRay(const Ray &ray) const;
    Collision traceSphere(const Vector3 &point) const;

    Color recursiveRayTracing(const Ray &ray, const int depth) const;

public:
    Scene();

    void add(const Object &object);
    void add(const Light &light);

    std::vector<Color> renderRayTracing(const View &view, int width, int height, int depth) const;
    std::vector<Color> renderSphereTracing(const View &view, int width, int height) const;
};

}

#endif
