#include "../include/RayTracingMath.h"

double solveLinearEquation(const double &a, const double &b) {
    if(std::abs(a)<EPSILON && std::abs(b)>EPSILON)
        return std::nan("");

    return -b/a;
}

std::pair<double, double> solveQuadraticEquation(const double &a, const double &b, const double &c) {
    //if(std::abs(a)<EPSILON)
    //    return std::make_pair(solveLinearEquation(b, c), std::nan(""));

    const double delta = b*b - 4*a*c;

    if(delta<-EPSILON)
        return std::make_pair(std::nan(""), std::nan(""));

    const double deltaSqrt = (delta>EPSILON) ? std::sqrt(delta) : 0;

    const double x1 = (-b + deltaSqrt)/(2*a);
    const double x2 = (-b - deltaSqrt)/(2*a);
    return std::make_pair(x1, x2);
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
