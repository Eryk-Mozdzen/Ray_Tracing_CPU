#include "RayTracingMath.h"

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

std::vector<double> solveQuarticEquation(const double &a, const double &b, const double &c, const double &d, const double &e) {
    const std::complex<double> p1 = 2*c*c*c - 9*b*c*d + 27*a*d*d + 27*b*b*e - 72*a*c*e;
    const std::complex<double> p2 = p1 + std::sqrt(-4*std::pow(c*c - 3*b*d + 12*a*e, 3) + p1*p1);
    const std::complex<double> p3 = (c*c - 3.*b*d + 12*a*e)/(3*a*std::pow(p2/2., 0.333)) + (std::pow(p2/2., 0.333))/(3*a);
    const std::complex<double> p4 = std::sqrt((b*b)/(4*a*a) - (2*c)/(3*a) + p3);
    const std::complex<double> p5 = (b*b)/(2*a*a) - (4*c)/(3*a) - p3;
    const std::complex<double> p6 = (-(b*b*b)/(a*a*a) + (4*b*c)/(a*a) - (8*d)/(a))/(4.*p4);

    std::vector<std::complex<double>> complexSolutions = {
        (-b/(4*a) - p4/2. - std::sqrt(p5 - p6)/2.),
        (-b/(4*a) - p4/2. + std::sqrt(p5 - p6)/2.),
        (-b/(4*a) + p4/2. - std::sqrt(p5 + p6)/2.),
        (-b/(4*a) + p4/2. + std::sqrt(p5 + p6)/2.),
    };

    std::vector<double> realSolutions;
    for(unsigned int i=0; i<complexSolutions.size(); i++) {
        if(std::abs(complexSolutions[i].imag())<EPSILON)
            realSolutions.push_back(complexSolutions[i].real());
    }

    return realSolutions;
}

double calculatePolynomial(const std::vector<double> &coeff, const double &x) {
    double y = 0;

    for(unsigned int i=0; i<coeff.size(); i++)
        y +=coeff[i]*std::pow(x, (double)(coeff.size()-i-1));

    return y;
}

Matrix solveLinearSystemCramersRule(const Matrix &A, const Matrix &b) {
    assert(A.getRows()==A.getCols());
    assert(A.getCols()==b.getRows());
    assert(b.getCols()==1);

    const unsigned int n = b.getRows();

    Matrix x(n, 1);

    const double W = Matrix::Det(A);

    if(std::abs(W)<EPSILON)
        return x;

    Matrix A_i;
    for(unsigned int i=0; i<n; i++) {
        A_i = A;

        for(unsigned int j=0; j<n; j++)
            A_i(j, i) = b(j, 0);

        x(i, 0) = Matrix::Det(A_i)/W;
    }

    return x;
}

Matrix solveLinearSystemJacobiMethod(const Matrix &A, const Matrix &b) {
    assert(A.getRows()==A.getCols());
    assert(A.getCols()==b.getRows());
    assert(b.getCols()==1);

    const unsigned int n = b.getRows();

    double diagonal = 0;
    double other = 0;
    for(unsigned int i=0; i<n; i++) {
        for(unsigned int j=0; j<n; j++) {
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

    for(unsigned int k=0; k<10; k++) {
        for(unsigned int i=0; i<n; i++) {
            double sigma = 0;
            for(unsigned int j=0; j<n; j++) {
                if(j!=i)
                    sigma +=A(i, j)*x(j, 0);
            }
            x(i, 0) = (b(i, 0) - sigma)/A(i, i);
        }
    }

    return x;
}
