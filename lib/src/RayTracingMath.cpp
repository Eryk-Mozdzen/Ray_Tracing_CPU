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

std::complex<double> complex_sqrt(const std::complex<double> &z) {
	return std::pow(z, 1./2.);
}

std::complex<double> complex_cbrt(const std::complex<double> &z) {
	return std::pow(z, 1./3.);
}

std::vector<double> solveQuarticEquation(double a_p, double b_p, double c_p, double d_p, double e_p) {
	const std::complex<double> a = a_p;
    const std::complex<double> b = b_p/a;
    const std::complex<double> c = c_p/a;
    const std::complex<double> d = d_p/a;
    const std::complex<double> e = e_p/a;

    const std::complex<double> Q1 = c*c - 3.*b*d + 12.*e;
    const std::complex<double> Q2 = 2.*c*c*c - 9.*b*c*d + 27.*d*d + 27.*b*b*e - 72.*c*e;
    const std::complex<double> Q3 = 8.*b*c - 16.*d - 2.*b*b*b;
    const std::complex<double> Q4 = 3.*b*b - 8.*c;

    const std::complex<double> Q5 = complex_cbrt(Q2/2. + complex_sqrt(Q2*Q2/4. - Q1*Q1*Q1));
    const std::complex<double> Q6 = (Q1/Q5 + Q5)/3.;
    const std::complex<double> Q7 = 2.*complex_sqrt(Q4/12. + Q6);

	std::vector<std::complex<double>> complexSolutions = {
		(-b - Q7 - complex_sqrt(4.*Q4/6. - 4.*Q6 - Q3/Q7))/4.,
		(-b - Q7 + complex_sqrt(4.*Q4/6. - 4.*Q6 - Q3/Q7))/4.,
		(-b + Q7 - complex_sqrt(4.*Q4/6. - 4.*Q6 + Q3/Q7))/4.,
		(-b + Q7 + complex_sqrt(4.*Q4/6. - 4.*Q6 + Q3/Q7))/4.
	};

    std::vector<double> realSolutions;
	realSolutions.reserve(4);

	for(const std::complex<double> &solution : complexSolutions) {
		if(std::abs(solution.imag())<EPSILON)
            realSolutions.push_back(solution.real());
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
