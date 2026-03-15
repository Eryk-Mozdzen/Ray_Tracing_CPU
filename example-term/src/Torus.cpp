#include "torus.h"
#include <cmath>
#include <vector>
#include <complex>
#include <algorithm>

Torus::Torus(rtrace::Vector3 center, double majorRadius, double minorRadius) : 
		majorRadius{majorRadius}, minorRadius{minorRadius} {

	transform.translate(center);
}

std::complex<double> complex_sqrt(const std::complex<double> &z) {
	return std::pow(z, 1./2.);
}

std::complex<double> complex_cbrt(const std::complex<double> &z) {
	return std::pow(z, 1./3.);
}

std::vector<double> findZerosPoly4(std::complex<double> a, std::complex<double> b, std::complex<double> c, std::complex<double> d, std::complex<double> e) {
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

rtrace::Collision Torus::intersect(const rtrace::Ray &ray) const {

	const rtrace::Ray relative = transform.convertWordToFrame(ray);

    // http://cosinekitty.com/raytrace/chapter13_torus.html
    const double A = majorRadius;
    const double B = minorRadius;
    const rtrace::Vector3 D = relative.origin;
    const rtrace::Vector3 E = relative.direction;
    const double G = 4.*A*A*(E.x*E.x + E.y*E.y);
    const double H = 8.*A*A*(D.x*E.x + D.y*E.y);
    const double I = 4.*A*A*(D.x*D.x + D.y*D.y);
    const double J = E*E;
    const double K = 2.*D*E;
    const double L = D*D + (A*A - B*B);

    std::vector<double> tSolutions = findZerosPoly4(J*J, 2.*J*K, 2.*J*L + K*K - G, 2.*K*L - H, L*L - I);
	
    if(!tSolutions.size()) {
		return rtrace::Collision();
	}

	std::remove_if(tSolutions.begin(), tSolutions.end(), [] (const double &solution) {
		return solution<rtrace::EPSILON;
	});

	if(!tSolutions.size()) {
		return rtrace::Collision();
	}

	const double t = *std::min_element(tSolutions.begin(), tSolutions.end());

	const rtrace::Vector3 P = relative.origin + t*relative.direction;
	const rtrace::Vector3 Q = rtrace::normalize(rtrace::Vector3(P.x, P.y, 0))*majorRadius;
	
	rtrace::Collision collision;

	collision.point = ray.origin + t*ray.direction;
	collision.normal = transform.getRotation()*rtrace::normalize(P - Q);
    collision.material = rtrace::Color::blue;
    collision.distance = t;
    collision.exist = true;

    return collision;
}

rtrace::Collision Torus::distance(const rtrace::Vector3 &point) const {
	return rtrace::Collision();
}
