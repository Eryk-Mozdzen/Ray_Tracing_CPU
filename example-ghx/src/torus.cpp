#include "torus.h"

Torus::Torus(rtrace::Vector3 center, double majorRadius, double minorRadius) : majorRadius{majorRadius}, minorRadius{minorRadius}, material{rtrace::Color::blue} {

	transform.translate(center);
}

/*  CollisionData CustomObject::intersect(const Ray &) const

    method parameter is Ray struct, 
    if user objects collide with that ray, should return correct CollisionData struct
    if not, should return not changed CollisionData struct		*/

rtrace::Collision Torus::intersect(const rtrace::Ray &ray) const {

	const rtrace::Ray relative = transform.convertWordToFrame(ray);

    // http://cosinekitty.com/raytrace/chapter13_torus.html
    const double A = majorRadius;
    const double B = minorRadius;
    const rtrace::Vector3 D = relative.origin;
    const rtrace::Vector3 E = relative.direction;
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

	const rtrace::Vector3 P = relative.origin + t*relative.direction;
	const rtrace::Vector3 Q = rtrace::normalize(rtrace::Vector3(P.x, P.y, 0))*majorRadius;
	
	rtrace::Collision collision;

	collision.point = ray.origin + t*ray.direction;
	collision.normal = transform.getRotation()*rtrace::normalize(P - Q);
    collision.material = material;
    collision.distance = t;
    collision.exist = true;

    return collision;
}

/*  CollisionData CustomObject::distance(const Vector3 &) const

    method parameter is Vector3 class, 
    method should return infromations about object int the nearest point */

rtrace::Collision Torus::distance(const rtrace::Vector3 &point) const {

	const rtrace::Vector3 P = transform.convertWordToFrame(point);
	const rtrace::Vector3 Q = rtrace::normalize(rtrace::Vector3(P.x, P.y, 0))*majorRadius;
	
	rtrace::Collision collision;

	collision.distance = rtrace::length(P - Q) - minorRadius;
    collision.point = point;
	collision.normal = transform.getRotation()*rtrace::normalize(P - Q);
    collision.material = material;
    collision.exist = (collision.distance<rtrace::EPSILON);

    return collision;
}
