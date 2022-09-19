#include "color.h"

rtrace::Color::Color() : r{0}, g{0}, b{0}, w{0} {

}

rtrace::Color::Color(const int &r, const int &g, const int &b) : w{255} {
	this->r = std::clamp(r, 0, 255);
	this->g = std::clamp(g, 0, 255);
	this->b = std::clamp(b, 0, 255);
}

rtrace::Color::Color(const int &r, const int &g, const int &b, const int &w) {
	this->r = std::clamp(r, 0, 255);
	this->g = std::clamp(g, 0, 255);
	this->b = std::clamp(b, 0, 255);
	this->w = std::clamp(w, 0, 255);
}

rtrace::Color rtrace::Color::operator*(const double &alpha) const {
	return Color(
        alpha*this->r,
        alpha*this->g,
        alpha*this->b,
		this->w
    );
}

rtrace::Color rtrace::Color::operator+(const rtrace::Color &rhs) const {
	return rtrace::Color(
		(this->w*this->r + rhs.w*rhs.r)/255,
		(this->w*this->g + rhs.w*rhs.g)/255,
		(this->w*this->b + rhs.w*rhs.b)/255,
		std::max(this->w, rhs.w)
	);
}

void rtrace::Color::operator+=(const rtrace::Color &rhs) {
	this->r = std::clamp((this->w*this->r + rhs.w*rhs.r)/255, 0, 255);
	this->g = std::clamp((this->w*this->g + rhs.w*rhs.g)/255, 0, 255);
	this->b = std::clamp((this->w*this->b + rhs.w*rhs.b)/255, 0, 255);
	this->w = std::max(this->w, rhs.w);
}

rtrace::Color rtrace::Color::lin(const rtrace::Color &color1, const rtrace::Color &color2, const double &alpha) {
    return (1. - alpha)*color1 + alpha*color2;
}

rtrace::Color rtrace::operator*(const double &lhs, const rtrace::Color &rhs) {
    return rhs*lhs;
}

std::ostream & rtrace::operator<<(std::ostream &lhs, const rtrace::Color &rhs) {
    lhs << "(";
    lhs << rhs.r << " ";
    lhs << rhs.g << " ";
    lhs << rhs.b;
    lhs << ")";
    return lhs;
}
