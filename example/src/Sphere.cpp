#include "Sphere.h"

Sphere::Sphere() {
    this->radius = 1;
}

Sphere::Sphere(const Vector3 &center, const double &radius) {
    this->radius = radius;
    this->transform.translate(center);
    this->material = Material(rand()%256, rand()%256, rand()%256);
}

Sphere::Sphere(const Vector3 &center, const double &radius, const Material &material) : Sphere(center, radius) {
    this->material = material;
}

sf::Color Sphere::getPixel(const Vector3 &point) const {
    if(!this->material.isTexture())
        return this->material.getColor();

    Vector3 relative = this->transform.getRelativeToTransform(point);

    Vector3 d = normalize(relative);

    double u = 0.5f + std::atan2(d.x, d.y)/(2.f*M_PI);
    double v = 0.5f - std::asin(d.z)/M_PI;

    return this->material.getColorAt(u, v);
}

const Transform3 & Sphere::getTransform() const {
    return this->transform;
}

const Material & Sphere::getMaterial() const {
    return this->material;
}

void Sphere::setTransform(const Transform3 &transform) {
    this->transform = transform;
}

void Sphere::setMaterial(const Material &material) {
    this->material = material;
}

CollisionData Sphere::intersect(const Ray &ray) const {
    CollisionData data;
    const Vector3 center = transform.getTranslation();

    const double a = 1;
    const double b = 2*(ray.direction*(ray.origin - center));
    const double c = length(ray.origin - center)*length(ray.origin - center) - this->radius*this->radius;

    const std::pair<double, double> tSolution = solveQuadraticEquation(a, b, c);

    double tNearestPositive;
            if(tSolution.first>EPSILON && tSolution.second<EPSILON)   tNearestPositive = tSolution.first;
    else if(tSolution.first<EPSILON && tSolution.second>EPSILON)   tNearestPositive = tSolution.second;
    else if(tSolution.first>EPSILON && tSolution.second>EPSILON)   tNearestPositive = std::min(tSolution.first, tSolution.second);
    else return data;

    data.point = ray.origin + tNearestPositive*normalize(ray.direction);
    data.normal = normalize(data.point - center);
    data.color = this->getPixel(data.point);
    data.material = this->getMaterial();
    data.distance = length(data.point - ray.origin);
    data.exist = true;

    return data;
}

CollisionData Sphere::distance(const Vector3 &point) const {
    CollisionData data;
    const double R = this->radius;
    const Vector3 center = this->transform.getTranslation();

    data.distance = length(point - this->transform.getTranslation()) - this->radius;
    data.point = point;
    data.normal = normalize(data.point - center);
    data.color = this->getPixel(normalize(point-center)*R + center);
    data.material = this->material;
    data.exist = (data.distance<EPSILON);
    
    return data;
}

