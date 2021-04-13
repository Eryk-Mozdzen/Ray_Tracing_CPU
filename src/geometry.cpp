#include "../include/geometry.h"

Line::Line() {}

Line::Line(Vector3 _point, Vector3 _direction) {
    point = _point;
    direction = normalize(_direction);
}

double distance(Line line, Vector3 point) {
    return length(line.direction^(line.point-point))/length(line.direction);
}

Plane::Plane() {}

Plane::Plane(double _A, double _B, double _C, double _D) {
    A = _A;
    B = _B;
    C = _C;
    D = _D;
}

Plane::Plane(Vector3 p, Vector3 n) {
    n = normalize(n);

    A = n.x;
    B = n.y;
    C = n.z;
    D = -A*p.x - B*p.y - C*p.z;
}

Plane::Plane(Vector3 p, Vector3 u, Vector3 v) : Plane(p, normalize(u^v)) {

}

Vector3 Plane::getNormal() {
    return normalize(Vector3(A, B, C));
}

bool Plane::intersect(Line line, Vector3 *point) {
    if(getNormal()*line.direction==0.f)
        return false;

    Vector3 p0 = Vector3(0, 0, -D/C);
    Vector3 p1 = Vector3(1, 0, -(D + A)/C);
    Vector3 p2 = Vector3(1, 1, -(D + A + B)/C);

    Vector3 p01 = p1 - p0;
    Vector3 p02 = p2 - p0;

    double t = -(((p01^p02)*(line.point - p0))/(line.direction*(p01^p02)));

    *point = line.point+t*line.direction;

    return true;
}




