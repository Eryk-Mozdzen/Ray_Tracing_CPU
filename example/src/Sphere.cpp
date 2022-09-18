#include "Sphere.h"

Sphere::Sphere(const Vector3 &center, const double &radius, const Material &material) : radius{radius} {
	this->material = material;
	this->transform.translate(center);
}

sf::Color Sphere::getPixel(const Vector3 &point) const {
    if(!this->material.isTexture())
        return this->material.getColor();

    Vector3 relative = this->transform.getRelativeToTransform(point);   // get position relative to base coordinates

    Vector3 d = normalize(relative);

    double u = 0.5f + std::atan2(d.x, d.y)/(2.f*M_PI);
    double v = 0.5f - std::asin(d.z)/M_PI;

    return this->material.getColorAt(u, v);
}

/*  CollisionData CustomObject::intersect(const Ray &) const

    method parameter is Ray struct, 
    if user objects collide with that ray, should return correct CollisionData struct
    if not, should return not changed CollisionData struct  */

CollisionData Sphere::intersect(const Ray &ray) const {
    CollisionData data;                     // construct fills struct with correct fields
    const Vector3 center = transform.getTranslation();

    const double a = 1;
    const double b = 2*(ray.direction*(ray.origin - center));
    const double c = length(ray.origin - center)*length(ray.origin - center) - this->radius*this->radius;

    const std::pair<double, double> tSolution = solveQuadraticEquation(a, b, c);

    double tNearestPositive;
            if(tSolution.first>EPSILON && tSolution.second<EPSILON)   tNearestPositive = tSolution.first;
    else if(tSolution.first<EPSILON && tSolution.second>EPSILON)   tNearestPositive = tSolution.second;
    else if(tSolution.first>EPSILON && tSolution.second>EPSILON)   tNearestPositive = std::min(tSolution.first, tSolution.second);
    else return data;   // return not changed if there are not solutions

    data.point = ray.origin + tNearestPositive*normalize(ray.direction);    // set collision point
    data.normal = normalize(data.point - center);                           // set surface notrmal vector in collision point
    data.color = this->getPixel(data.point);                                // set color of the object in collision point
    data.material = this->getMaterial();                                    // set material of the objects
    data.distance = length(data.point - ray.origin);                        // set distance from ray origin to collision point
    data.exist = true;                                                      // collision occured? set to true

    return data;    // return collision data
}

/*  CollisionData CustomObject::distance(const Vector3 &) const

    method parameter is Vector3 class, 
    method should return infromations about object int the nearest point */

CollisionData Sphere::distance(const Vector3 &point) const {
    CollisionData data;             // construct fills struct with correct fields
    const double R = this->radius;
    const Vector3 center = this->transform.getTranslation();

    data.distance = length(point - this->transform.getTranslation()) - this->radius;    // set distance from surface to point (with sign)
    data.point = point;                                                                 // set point, where calculations are
    data.normal = normalize(data.point - center);                                       // set normal in the nearest point
    data.color = this->getPixel(normalize(point-center)*R + center);                    // set point, where calculations are
    data.material = this->material;                                                     // set material of the objects
    data.exist = (data.distance<EPSILON);                                               // set true if point is near enough
    
    return data;    // return collision data
}

