#ifndef RTRACE_SCENE_HPP
#define RTRACE_SCENE_HPP

#include <functional>
#include <vector>

#include "rtrace/Color.hpp"
#include "rtrace/Light.hpp"
#include "rtrace/Object.hpp"
#include "rtrace/View.hpp"

namespace rtrace {

/*
RenderWindow class
In this place heppend all magic.
It represent application window and all informations about scene with objects and light.
*/

class Scene {
    std::vector<std::reference_wrapper<const Object>> objects;
    std::vector<std::reference_wrapper<const Light>> lights;

    Collision traceRay(const Ray &) const;
    Collision traceSphere(const Vector3 &) const;

    Color recursiveRayTracing(const Ray &, int) const;

public:
    Scene();

    void add(const Object &);
    void add(const Light &);

    std::vector<Color> renderRayTracing(const View &, int, int, int) const;
    std::vector<Color> renderSphereTracing(const View &, int, int) const;
};

}

#endif
