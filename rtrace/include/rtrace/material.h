#pragma once

#include <rtrace/color.h>

namespace rtrace {

	/*  Material class
		This class contains all required informations 
		about material properties in render algorith.     */

	struct Material {
		double ambient, diffuse, specular, shininess, reflection; //phong model
		Color color;

		Material();
		Material(const Color&);
		Material(const Color&, const double&, const double&, const double&, const double&, const double&);
		Material(const int&, const int&, const int&);
	};

}
