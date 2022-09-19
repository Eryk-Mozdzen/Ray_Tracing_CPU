#include "material.h"

rtrace::Material::Material() : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{255, 0, 0} {
	
}

rtrace::Material::Material(const unsigned int &r, const unsigned int &g, const unsigned int &b) : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{(sf::Uint8)r, (sf::Uint8)g, (sf::Uint8)b} {
	
}
