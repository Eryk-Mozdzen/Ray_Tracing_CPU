#include "collision.h"

rtrace::Collision::Collision() {
    this->normal = rtrace::Vector3(1, 0, 0);
    this->distance = 1000000;
    this->exist = false;
}

rtrace::Collision rtrace::Collision::min(const rtrace::Collision &a, const rtrace::Collision &b) {
    return (a.distance<b.distance) ? a : b;
}

rtrace::Collision rtrace::Collision::smin(const rtrace::Collision &a, const rtrace::Collision &b, const double &k) {
    Collision data = Collision::min(a, b);

    const double h = std::max(k - std::abs(a.distance-b.distance), 0.)/k;
    data.distance = std::min(a.distance, b.distance) - h*h*h*k*0.167;

    if(data.distance<rtrace::EPSILON) {
        data.exist = true;
    }

    if(h>0) {
        const double m = a.distance/(a.distance + b.distance);
		
		data.material.color = color_interpolation(a.material.color, b.material.color, m);
    }
    
    return data;
}
