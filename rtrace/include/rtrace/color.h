#pragma once

#include <ostream>

namespace rtrace {

	struct Color {
		int r, g, b, w;

		Color();
		Color(const int&, const int&, const int&);
		Color(const int&, const int&, const int&, const int&);

		Color operator*(const double&) const;
		Color operator+(const Color&) const;
		void operator+=(const Color&);

		static Color lin(const Color&, const Color&, const double&);

		static const Color red;
		static const Color green;
		static const Color blue;
		static const Color white;
		static const Color black;
		static const Color cyan;
		static const Color magenta;
		static const Color orange;
		static const Color pink;
	};
	
	Color operator*(const double&, const Color&);
	std::ostream & operator<<(std::ostream&, const Color&);
}
