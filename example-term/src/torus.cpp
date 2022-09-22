#include "torus.h"

Torus::Torus(rtrace::Vector3 center, double majorRadius, double minorRadius) : 
		rtrace::BoundingBox{transform, 
			rtrace::Vector3(-majorRadius-minorRadius, -majorRadius-minorRadius, -minorRadius),
			rtrace::Vector3(majorRadius+minorRadius, majorRadius+minorRadius, minorRadius)},
		majorRadius{majorRadius}, minorRadius{minorRadius} {

	transform.translate(center);
}

rtrace::Collision Torus::intersect(const rtrace::Ray &ray) const {

	if(!rtrace::BoundingBox::intersect(ray))
		return rtrace::Collision();

	const rtrace::Vector3 origin = transform.getRelativeToTransform(ray.origin);
	const rtrace::Vector3 dir = rtrace::solveLinearSystem(transform.getRotation(), ray.direction);

    // http://cosinekitty.com/raytrace/chapter13_torus.html
    const double A = majorRadius;
    const double B = minorRadius;
    const rtrace::Vector3 D = origin;
    const rtrace::Vector3 E = dir;
    const double G = 4.*A*A*(E.x*E.x + E.y*E.y);
    const double H = 8.*A*A*(D.x*E.x + D.y*E.y);
    const double I = 4.*A*A*(D.x*D.x + D.y*D.y);
    const double J = E*E;
    const double K = 2.*D*E;
    const double L = D*D + (A*A - B*B);

    std::vector<double> tSolutions = rtrace::findZerosPoly4(J*J, 2.*J*K, 2.*J*L + K*K - G, 2.*K*L - H, L*L - I);
	
    if(!tSolutions.size()) {
		return rtrace::Collision();
	}

	std::remove_if(tSolutions.begin(), tSolutions.end(), [] (const double &solution) {
		return solution<rtrace::EPSILON;
	});

	if(!tSolutions.size()) {
		return rtrace::Collision();
	}

	const double t = *std::min_element(tSolutions.begin(), tSolutions.end());

	const rtrace::Vector3 P = origin + t*dir;
	const rtrace::Vector3 Q = rtrace::normalize(rtrace::Vector3(P.x, P.y, 0))*majorRadius;
	
	rtrace::Collision collision;

	collision.point = ray.origin + t*ray.direction;
	collision.normal = transform.getRotation()*rtrace::normalize(P - Q);
    collision.material = rtrace::Color::blue;
    collision.distance = t;
    collision.exist = true;

    return collision;
}

rtrace::Collision Torus::distance(const rtrace::Vector3 &point) const {
	return rtrace::Collision();
}
