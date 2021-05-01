#include "Torus.h"

Torus::Torus() {
    this->majorRadius = 2;
    this->minorRadius = 1;
}

Torus::Torus(const Vector3 &center, const double &majorRadius, const double &minorRadius) {
    this->transform.translate(center);

    this->majorRadius = majorRadius;
    this->minorRadius = minorRadius;

    this->material = Material(rand()%255, rand()%255, rand()%255);
}

Torus::Torus(const Vector3 &center, const double &majorRadius, const double &minorRadius, const Material &material) : Torus(center, majorRadius, minorRadius) {
    this->material = material;
}

sf::Color Torus::getPixel() const {
    return this->material.getColor();
}

Vector3 Torus::getNormal(const Vector3 &point) const {
    const double alpha = (this->majorRadius*this->majorRadius)/std::sqrt(point.x*point.x + point.y*point.y);

    return normalize(point - alpha*Vector3(point.x, point.y, 0));
    //return normalize(point - this->majorRadius*normalize(Vector3(point.x, point.y, 0)));
}

CollisionData Torus::intersect(const Ray &ray) const {
    CollisionData data;
    const Vector3 center = this->transform.getTranslation();
    const Ray rayRelative(ray.origin - center, ray.direction);

    //http://cosinekitty.com/raytrace/chapter13_torus.html
    const double A = this->majorRadius;
    const double B = this->minorRadius;
    const Vector3 D = rayRelative.origin;
    const Vector3 E = rayRelative.direction;
    const double G = 4*A*A*(E.x*E.x + E.y*E.y);
    const double H = 8*A*A*(D.x*E.x + D.y*E.y);
    const double I = 4*A*A*(D.x*D.x + D.y*D.y);
    const double J = E*E;
    const double K = 2*D*E;
    const double L = D*D + (A*A - B*B);

    std::vector<double> tSolutions = solveQuarticEquation(J*J, 2*J*K, 2*J*L + K*K - G, 2*K*L - H, L*L - I);
    //std::vector<double> tSolutions = approxPolynomialRoots({J*J, 2*J*K, 2*J*L + K*K - G, 2*K*L - H, L*L - I}, -100, 100, 1);
    if(tSolutions.size()==0)
        return data;

    double tNearestPositive = 1E+9;
    bool exist = false;
    for(unsigned int i=0; i<tSolutions.size(); i++) {
        if(tSolutions[i]>EPSILON && tSolutions[i]<tNearestPositive) {
            tNearestPositive = tSolutions[i];
            exist = true;
        }
    }
    if(!exist)
        return data;

    data.point = rayRelative.origin + tNearestPositive*normalize(rayRelative.direction);
    data.normal = this->getNormal(data.point-center);
    data.color = this->getPixel();
    data.material = this->material;
    data.distance = length(data.point - rayRelative.origin);
    data.exist = true;

    return data;
}

CollisionData Torus::distance(const Vector3 &point) const {
    CollisionData data;
    const double R = this->majorRadius;
    const double r = this->minorRadius;
    const Vector3 center = this->transform.getTranslation();

    Vector3 t = point - center;

    data.distance = length(Vector3(length(Vector3(t.x, t.y, 0)) - R, 0, t.z)) - r;
    data.point = point;
    data.normal = this->getNormal(data.point-center);
    data.color = this->material.getColor();
    data.material = this->material;
    data.exist = (data.distance<EPSILON);

    return data;
}
