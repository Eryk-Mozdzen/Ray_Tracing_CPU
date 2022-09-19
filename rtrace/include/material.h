#pragma once

#include "color.h"

namespace rtrace {

	/*  Material class
		This class contains all required informations 
		about material properties in render algorith.     */

	struct Material {
		double ambient, diffuse, specular, shininess, reflection; //phong model
		Color color;

		Material();
		Material(const int&, const int&, const int&);
	};

}
