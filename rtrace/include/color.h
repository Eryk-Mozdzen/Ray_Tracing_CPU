#pragma once

#include <ostream>
#include <algorithm>

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
	};
	
	Color operator*(const double&, const Color&);
	std::ostream & operator<<(std::ostream&, const Color&);
}