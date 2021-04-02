#include "geometry.h"

Line::Line() {}

Line::Line(Vector3d _point, Vector3d _direction) {
    point = _point;
    direction = normalize(_direction);
}

double distance(Line line, Vector3d point) {
    return length(cross(line.direction, line.point-point))/length(line.direction);
}

Plane::Plane() {}

Plane::Plane(double _A, double _B, double _C, double _D) {
    A = _A;
    B = _B;
    C = _C;
    D = _D;
}

Plane::Plane(Vector3d p, Vector3d n) {
    n = normalize(n);

    A = n.x;
    B = n.y;
    C = n.z;
    D = -A*p.x - B*p.y - C*p.z;
}

Plane::Plane(Vector3d p, Vector3d u, Vector3d v) {
    Vector3d n = normalize(cross(u, v));

    A = n.x;
    B = n.y;
    C = n.z;
    D = -A*p.x - B*p.y - C*p.z;
}

Vector3d Plane::getNormal() {
    return normalize(Vector3d(A, B, C));
}

bool Plane::intersect(Line line, Vector3d *point) {
    if(dot(getNormal(), line.direction)==0.f)
        return false;

    Vector3d p0 = Vector3d(0, 0, -D/C);
    Vector3d p1 = Vector3d(1, 0, -(D + A)/C);
    Vector3d p2 = Vector3d(1, 1, -(D + A + B)/C);

    Vector3d p01 = p1 - p0;
    Vector3d p02 = p2 - p0;

    double t = -dot(cross(p01, p02), line.point - p0)/dot(line.direction, cross(p01, p02));

    *point = line.point+t*line.direction;

    return true;
}

Vector3d cross(Vector3d a, Vector3d b) {
    return Vector3d(
        a.y*b.z - a.z*b.y,
        -(a.x*b.z - a.z*b.x),
        a.x*b.y - a.y*b.x
    );
}

double dot(Vector3d a, Vector3d b) {
    return (
        a.x*b.x +
        a.y*b.y +
        a.z*b.z
    );
}

double triple(Vector3d a, Vector3d b, Vector3d c) {
    return dot(a, cross(b, c));
}

double length(Vector3d v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector3d normalize(Vector3d v) {
    double len = length(v);
    if(len!=0)
        return (v/len);
    else
        return v;
}

void print(Vector3d v) {
    std::cout << v.x << '\t' << v.y << '\t' << v.z << '\n';
}

Vector3d rotate(Vector3d axis, Vector3d v, double theta) {
    axis = normalize(axis);

    double c = std::cos(theta);
    double s = std::sin(theta);

    //Rodrigues' rotation formula
    return (c*v + s*cross(axis, v) + dot(axis, v)*(1.f-c)*axis);
}




