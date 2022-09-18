#include "sphere.h"

Sphere::Sphere(const rtrace::Vector3 &center, const double &radius, const rtrace::Material &material) : radius{radius} {
	this->material = material;
	this->transform.translate(center);
}

sf::Color Sphere::getPixel(const rtrace::Vector3 &point) const {
    if(!this->material.isTexture())
        return this->material.getColor();

    rtrace::Vector3 relative = this->transform.getRelativeToTransform(point);   // get position relative to base coordinates

    rtrace::Vector3 d = rtrace::normalize(relative);

    double u = 0.5f + std::atan2(d.x, d.y)/(2.f*M_PI);
    double v = 0.5f - std::asin(d.z)/M_PI;

    return this->material.getColorAt(u, v);
}

/*  CollisionData CustomObject::intersect(const Ray &) const

    method parameter is Ray struct, 
    if user objects collide with that ray, should return correct CollisionData struct
    if not, should return not changed CollisionData struct  */

rtrace::CollisionData Sphere::intersect(const rtrace::Ray &ray) const {
    rtrace::CollisionData data;                     // construct fills struct with correct fields
    const rtrace::Vector3 center = transform.getTranslation();

    const double a = 1;
    const double b = 2*(ray.direction*(ray.origin - center));
    const double c = length(ray.origin - center)*length(ray.origin - center) - this->radius*this->radius;

    const std::pair<double, double> tSolution = rtrace::solveQuadraticEquation(a, b, c);

    double tNearestPositive;
    if(tSolution.first>rtrace::EPSILON && tSolution.second<rtrace::EPSILON)   tNearestPositive = tSolution.first;
    else if(tSolution.first<rtrace::EPSILON && tSolution.second>rtrace::EPSILON)   tNearestPositive = tSolution.second;
    else if(tSolution.first>rtrace::EPSILON && tSolution.second>rtrace::EPSILON)   tNearestPositive = std::min(tSolution.first, tSolution.second);
    else return data;   // return not changed if there are not solutions

    data.point = ray.origin + tNearestPositive*rtrace::normalize(ray.direction);    // set collision point
    data.normal = rtrace::normalize(data.point - center);                           // set surface notrmal vector in collision point
    data.color = this->getPixel(data.point);                                		// set color of the object in collision point
    data.material = this->getMaterial();                                    		// set material of the objects
    data.distance = rtrace::length(data.point - ray.origin);                        // set distance from ray origin to collision point
    data.exist = true;                                                      		// collision occured? set to true

    return data;
}

/*  CollisionData CustomObject::distance(const Vector3 &) const

    method parameter is Vector3 class, 
    method should return infromations about object int the nearest point */

rtrace::CollisionData Sphere::distance(const rtrace::Vector3 &point) const {
    rtrace::CollisionData data;             // construct fills struct with correct fields
    const double R = this->radius;
    const rtrace::Vector3 center = this->transform.getTranslation();

    data.distance = rtrace::length(point - this->transform.getTranslation()) - this->radius;    // set distance from surface to point (with sign)
    data.point = point;                                                                 		// set point, where calculations are
    data.normal = rtrace::normalize(data.point - center);                                       // set normal in the nearest point
    data.color = this->getPixel(rtrace::normalize(point-center)*R + center);                    // set point, where calculations are
    data.material = this->material;                                                     		// set material of the objects
    data.exist = (data.distance<rtrace::EPSILON);                                               // set true if point is near enough
    
    return data;
}

