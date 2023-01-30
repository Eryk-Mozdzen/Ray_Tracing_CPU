#include <rtrace/collision.h>
#include <cmath>

namespace rtrace {

	Collision::Collision() {
		this->normal = Vector3(1, 0, 0);
		this->distance = 1000000;
		this->exist = false;
	}

	Collision Collision::min(const Collision &a, const Collision &b) {
		return (a.distance<b.distance) ? a : b;
	}

	Collision Collision::smin(const Collision &a, const Collision &b, const double &k) {
		Collision data = Collision::min(a, b);

		const double h = std::max(k - std::abs(a.distance-b.distance), 0.)/k;
		data.distance = std::min(a.distance, b.distance) - h*h*h*k*0.167;

		if(data.distance<EPSILON) {
			data.exist = true;
		}

		if(h>0) {
			const double m = a.distance/(a.distance + b.distance);
			
			data.material.color = Color::lin(a.material.color, b.material.color, m);
		}
		
		return data;
	}

}
