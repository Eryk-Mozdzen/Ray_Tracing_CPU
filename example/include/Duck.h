#ifndef DUCK_H
#define DUCK_H

#include "RayTracing.h"
#include <vector>

#include "Sphere.h"
#include "Box.h"

struct Duck {
    Material washes;
    Material skin;
    Material eye;
    std::vector<Object*> body;

    Duck();

    void construct(RenderScene&) const;
};

#endif