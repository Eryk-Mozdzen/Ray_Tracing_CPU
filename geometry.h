#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

typedef sf::Vector3<double> Vector3d;

class Line {
public:
    Vector3d point;
    Vector3d direction;

    Line();
    Line(Vector3d, Vector3d);
};

class Plane {
public:
    double A, B, C, D;

    Plane();
    Plane(double, double, double, double);
    Plane(Vector3d, Vector3d);
    Plane(Vector3d, Vector3d, Vector3d);
    Vector3d getNormal();
    bool intersect(Line, Vector3d*);
};

double distance(Line, Vector3d);
void print(Vector3d);
double length(Vector3d);
double dot(Vector3d, Vector3d);
double triple(Vector3d, Vector3d, Vector3d);
Vector3d cross(Vector3d, Vector3d);
Vector3d normalize(Vector3d);
Vector3d rotate(Vector3d, Vector3d, double);


