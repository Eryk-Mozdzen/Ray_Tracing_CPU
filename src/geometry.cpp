#include "../include/geometry.h"

Line::Line() {}

Line::Line(const Vector3 &point, const Vector3 &direction) {
    this->point = point;
    this->direction = normalize(direction);
}

double distance(const Line &line, const Vector3 &point) {
    return length(line.direction^(line.point-point))/length(line.direction);
}

Plane::Plane() {}

Plane::Plane(const double &A, const double &B, const double &C, const double &D) {
    this->A = A;
    this->B = B;
    this->C = C;
    this->D = D;
}

Plane::Plane(const Vector3 &p, const Vector3 &n) {
    Vector3 norm = normalize(n);

    A = norm.x;
    B = norm.y;
    C = norm.z;
    D = -A*p.x - B*p.y - C*p.z;
}

Plane::Plane(const Vector3 &p, const Vector3& u, const Vector3 &v) : Plane(p, normalize(u^v)) {}

Vector3 Plane::getNormal() const {
    return normalize(Vector3(A, B, C));
}

bool Plane::intersect(const Line &line, Vector3 *point) const {
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

Matrix solveLinearSystemCramersRule(const Matrix &A, const Matrix &b) {
    assert(A.getRows()==A.getCols());
    assert(A.getCols()==b.getRows());
    assert(b.getCols()==1);

    const int n = b.getRows();

    Matrix x(n, 1);

    const double W = Matrix::Det(A);

    if(std::abs(W)<EPSILON)
        return x;

    Matrix A_i;
    for(int i=0; i<n; i++) {
        A_i = A;

        for(int j=0; j<n; j++)
            A_i(j, i) = b(j, 0);

        x(i, 0) = Matrix::Det(A_i)/W;
    }

    return x;
}

Matrix solveLinearSystemJacobiMethod(const Matrix &A, const Matrix &b) {
    assert(A.getRows()==A.getCols());
    assert(A.getCols()==b.getRows());
    assert(b.getCols()==1);

    const int n = b.getRows();

    double diagonal = 0;
    double other = 0;
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(j!=i)
                other +=std::abs(A(i, j));
        }
        diagonal +=std::abs(A(i, i));
    }

    if(diagonal<other) {
        //std::cout << A << std:: endl;
        //std::cout << diagonal << "\t" << other << std::endl;
        //return solveLinearSystemCramersRule(A, b);
        return Matrix(n, 1);
    }

    Matrix x(n, 1);

    for(int k=0; k<10; k++) {
        for(int i=0; i<n; i++) {
            double sigma = 0;
            for(int j=0; j<n; j++) {
                if(j!=i)
                    sigma +=A(i, j)*x(j, 0);
            }
            x(i, 0) = (b(i, 0) - sigma)/A(i, i);
        }
    }

    return x;
}



