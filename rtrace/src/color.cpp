#include <rtrace/color.h>
#include <algorithm>

namespace rtrace {

	const Color Color::red(255, 0, 0);
	const Color Color::green(0, 255, 0);
	const Color Color::blue(0, 0, 255);
	const Color Color::white(255, 255, 255);
	const Color Color::black(0, 0, 0);
	const Color Color::cyan(0, 255, 255);
	const Color Color::magenta(255, 0, 255);
	const Color Color::orange(255, 140, 0);
	const Color Color::pink(255, 105, 180);

	Color::Color() : r{0}, g{0}, b{0}, w{0} {

	}

	Color::Color(const int &r, const int &g, const int &b) : w{255} {
		this->r = std::clamp(r, 0, 255);
		this->g = std::clamp(g, 0, 255);
		this->b = std::clamp(b, 0, 255);
	}

	Color::Color(const int &r, const int &g, const int &b, const int &w) {
		this->r = std::clamp(r, 0, 255);
		this->g = std::clamp(g, 0, 255);
		this->b = std::clamp(b, 0, 255);
		this->w = std::clamp(w, 0, 255);
	}

	Color Color::operator*(const double &alpha) const {
		return Color(
			alpha*this->r,
			alpha*this->g,
			alpha*this->b,
			this->w
		);
	}

	Color Color::operator+(const Color &rhs) const {
		return Color(
			(this->w*this->r + rhs.w*rhs.r)/255,
			(this->w*this->g + rhs.w*rhs.g)/255,
			(this->w*this->b + rhs.w*rhs.b)/255,
			std::max(this->w, rhs.w)
		);
	}

	void Color::operator+=(const Color &rhs) {
		this->r = std::clamp((this->w*this->r + rhs.w*rhs.r)/255, 0, 255);
		this->g = std::clamp((this->w*this->g + rhs.w*rhs.g)/255, 0, 255);
		this->b = std::clamp((this->w*this->b + rhs.w*rhs.b)/255, 0, 255);
		this->w = std::max(this->w, rhs.w);
	}

	Color Color::lin(const Color &color1, const Color &color2, const double &alpha) {
		return (1. - alpha)*color1 + alpha*color2;
	}

	Color operator*(const double &lhs, const Color &rhs) {
		return rhs*lhs;
	}

	std::ostream & operator<<(std::ostream &lhs, const Color &rhs) {
		lhs << "(";
		lhs << rhs.r << " ";
		lhs << rhs.g << " ";
		lhs << rhs.b;
		lhs << ")";
		return lhs;
	}

}
