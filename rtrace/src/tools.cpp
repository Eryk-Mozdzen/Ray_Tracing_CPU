#include <rtrace/tools.h>

rtrace::Matrix<1, 3> rtrace::transposition(const rtrace::Vector3 &vec) {
	rtrace::Matrix<1, 3> result;

	result(0, 0) = vec.x;
	result(0, 1) = vec.y;
	result(0, 2) = vec.z;

	return result;
}

double findZeroPoly1(double a, double b) {
    if(std::abs(a)<rtrace::EPSILON)
        return 0;

    return -b/a;
}

std::vector<double> rtrace::findZerosPoly2(double a, double b, double c) {
    const double delta = b*b - 4.*a*c;

    if(delta<0.)
        return {};

    const double deltaSqrt = std::sqrt(delta);

    const double x1 = (-b + deltaSqrt)/(2.*a);
    const double x2 = (-b - deltaSqrt)/(2.*a);

    return {x1, x2};
}

std::complex<double> complex_sqrt(const std::complex<double> &z) {
	return std::pow(z, 1./2.);
}

std::complex<double> complex_cbrt(const std::complex<double> &z) {
	return std::pow(z, 1./3.);
}

std::vector<double> rtrace::findZerosPoly4(std::complex<double> a, std::complex<double> b, std::complex<double> c, std::complex<double> d, std::complex<double> e) {
	b /=a;
	c /=a;
	d /=a;
	e /=a;

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
		if(std::abs(solution.imag())<rtrace::EPSILON)
            realSolutions.push_back(solution.real());
	}

    return realSolutions;
}

rtrace::Vector3 rtrace::operator*(const rtrace::Matrix<3, 3> &matrix, const rtrace::Vector3 &vec) {
    rtrace::Vector3 result;

	result.x = matrix(0, 0)*vec.x + matrix(0, 1)*vec.y + matrix(0, 2)*vec.z;
	result.y = matrix(1, 0)*vec.x + matrix(1, 1)*vec.y + matrix(1, 2)*vec.z;
	result.z = matrix(2, 0)*vec.x + matrix(2, 1)*vec.y + matrix(2, 2)*vec.z;

	return result;
}

rtrace::Matrix<3, 3> rtrace::operator*(const rtrace::Vector3 &vec, const rtrace::Matrix<1, 3> &matrix) {
	rtrace::Matrix<3, 1> mvec;
	mvec(0, 0) = vec.x;
	mvec(1, 0) = vec.y;
	mvec(2, 0) = vec.z;

	return mvec*matrix;
}

rtrace::Vector3 rtrace::toVec(const rtrace::Matrix<3, 1> &matrix) {
	return rtrace::Vector3(
		matrix(0, 0),
		matrix(1, 0),
		matrix(2, 0)
	);
}

rtrace::Matrix<3, 1> rtrace::toMat(const rtrace::Vector3 &vec) {
	rtrace::Matrix<3, 1> mat;

	mat(0, 0) = vec.x;
	mat(1, 0) = vec.y;
	mat(2, 0) = vec.z;

	return mat;
}
