#pragma once

#include <SFML/Graphics.hpp>

namespace rtrace {

	/*  Material class
		This class contains all required informations 
		about material properties in render algorith.     */

	struct Material {
		double ambient, diffuse, specular, shininess, reflection; //phong model
		sf::Color color;

		Material();
		Material(const unsigned int&, const unsigned int&, const unsigned int&);
	};

}
