#include "torus.h"

Torus::Torus(const rtrace::Vector3 &center, const double &majorRadius, const double &minorRadius, const rtrace::Material &material) : 
		majorRadius{majorRadius}, minorRadius{minorRadius} {

	this->transform.translate(center);

	this->material = material;
}

rtrace::Vector3 Torus::getNormal(const rtrace::Vector3 &P) const {
	const rtrace::Vector3 Q = rtrace::normalize(rtrace::Vector3(P.x, P.y, 0))*this->majorRadius;
	return rtrace::normalize(P - Q);
}

/*  CollisionData CustomObject::intersect(const Ray &) const

    method parameter is Ray struct, 
    if user objects collide with that ray, should return correct CollisionData struct
    if not, should return not changed CollisionData struct		*/

rtrace::CollisionData Torus::intersect(const rtrace::Ray &ray) const {
	const rtrace::Vector3 origin = this->transform.getRotation()*(ray.origin - this->transform.getTranslation());
	//const Vector3 origin = this->transform.getRelativeToTransform(ray.origin);
	const rtrace::Vector3 dir = this->transform.getRotation()*ray.direction;

    // http://cosinekitty.com/raytrace/chapter13_torus.html
    const double A = this->majorRadius;
    const double B = this->minorRadius;
    const rtrace::Vector3 D = origin;
    const rtrace::Vector3 E = dir;
    const double G = 4.*A*A*(E.x*E.x + E.y*E.y);
    const double H = 8.*A*A*(D.x*E.x + D.y*E.y);
    const double I = 4.*A*A*(D.x*D.x + D.y*D.y);
    const double J = E*E;
    const double K = 2.*D*E;
    const double L = D*D + (A*A - B*B);

    std::vector<double> tSolutions = rtrace::solveQuarticEquation(J*J, 2.*J*K, 2.*J*L + K*K - G, 2.*K*L - H, L*L - I);
	
	rtrace::CollisionData data;

    if(!tSolutions.size()) {
		return data;
	}

	std::remove_if(tSolutions.begin(), tSolutions.end(), [] (const double &solution) {
		return solution<=rtrace::EPSILON;
	});

	if(!tSolutions.size()) {
		return data;
	}

	const double t = *std::min_element(tSolutions.begin(), tSolutions.end());

	data.point = ray.origin + t*ray.direction;
    data.normal = this->transform.getRotation().getInverse()*this->getNormal(origin + t*dir);
    data.color = this->material.getColor();
    data.material = this->material;
    data.distance = t;
    data.exist = true;

    return data;
}

/*  CollisionData CustomObject::distance(const Vector3 &) const

    method parameter is Vector3 class, 
    method should return infromations about object int the nearest point */

rtrace::CollisionData Torus::distance(const rtrace::Vector3 &point) const {
    const double R = this->majorRadius;
    const double r = this->minorRadius;

	const rtrace::Vector3 P = this->transform.getRotation()*(point - this->transform.getTranslation());
	const rtrace::Vector3 Q = rtrace::normalize(rtrace::Vector3(P.x, P.y, 0))*R;
	
	rtrace::CollisionData data;

	data.distance = rtrace::length(P - Q) - r;		// set distance from surface to point (with sign)
    data.point = point;                     		// set point, where calculations are
    data.normal = this->getNormal(P);				// set normal in the nearest point
    data.color = this->material.getColor();			// set color in the nearest point
    data.material = this->material;         		// set material of the objects
    data.exist = (data.distance<rtrace::EPSILON);   // set true if point is near enough

    return data;
}
