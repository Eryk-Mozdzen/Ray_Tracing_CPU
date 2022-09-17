#include "Torus.h"

Torus::Torus(const Vector3 &center, const double &majorRadius, const double &minorRadius, const Material &material) : 
		majorRadius{majorRadius}, minorRadius{minorRadius}, material{material} {

	this->transform.translate(center);
}

Vector3 Torus::getNormal(const Vector3 &P) const {
	const Vector3 Q = normalize(Vector3(P.x, P.y, 0))*this->majorRadius;
	return normalize(P - Q);
}

/*  CollisionData CustomObject::intersect(const Ray &) const

    method parameter is Ray struct, 
    if user objects collide with that ray, should return correct CollisionData struct
    if not, should return not changed CollisionData struct,  

    If user want to test it, be free, but doubleing point errors are very big here
    so overall donut is not looking good (but idea of this method is the same).    */

CollisionData Torus::intersect(const Ray &ray) const {
    CollisionData data;

	const Vector3 center = this->transform.getTranslation();

    // http://cosinekitty.com/raytrace/chapter13_torus.html
    const double A = this->majorRadius;
    const double B = this->minorRadius;
    const Vector3 D = ray.origin - center;
    const Vector3 E = ray.direction;
    const double G = 4.*A*A*(E.x*E.x + E.y*E.y);
    const double H = 8.*A*A*(D.x*E.x + D.y*E.y);
    const double I = 4.*A*A*(D.x*D.x + D.y*D.y);
    const double J = E*E;
    const double K = 2.*D*E;
    const double L = D*D + (A*A - B*B);

    std::vector<double> tSolutions = solveQuarticEquation(J*J, 2.*J*K, 2.*J*L + K*K - G, 2.*K*L - H, L*L - I);

    if(!tSolutions.size()) {
		return data;
	}

	std::remove_if(tSolutions.begin(), tSolutions.end(), [] (const double &solution) {
		return solution<=EPSILON;
	});

	if(!tSolutions.size()) {
		return data;
	}

	const double tNearestPositive = *std::min_element(tSolutions.begin(), tSolutions.end());

	data.point = tNearestPositive*ray.direction + ray.origin;
    data.normal = this->getNormal(data.point - center);
    data.color = this->material.getColor();
    data.material = this->material;
    data.distance = tNearestPositive;
    data.exist = true;

    return data;
}

/*  CollisionData CustomObject::distance(const Vector3 &) const

    method parameter is Vector3 class, 
    method should return infromations about object int the nearest point */

CollisionData Torus::distance(const Vector3 &point) const {
    CollisionData data;             // construct fills struct with correct fields
    const double R = this->majorRadius;
    const double r = this->minorRadius;
    const Vector3 center = this->transform.getTranslation();

    Vector3 t = point - center;

    data.distance = length(Vector3(length(Vector3(t.x, t.y, 0)) - R, 0, t.z)) - r;  // set distance from surface to point (with sign)
    data.point = point;                                                             // set point, where calculations are
    data.normal = this->getNormal(data.point-center);                               // set normal in the nearest point
    data.color = this->material.getColor();                                         // set color in the nearest point
    data.material = this->material;                                                 // set material of the objects
    data.exist = (data.distance<EPSILON);                                           // set true if point is near enough

    return data;    // return collision data
}
