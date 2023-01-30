#include <rtrace/material.h>

namespace rtrace {

	Material::Material() : 
			ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.5}, color{255, 0, 0} {
		
	}

	Material::Material(const Color &color) : 
			ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.5}, color{color} {

	}

	Material::Material(const Color &color, const double &ambient, const double &diffuse, const double &specular, const double &shininess, const double &reflection) : 
			ambient{ambient}, diffuse{diffuse}, specular{specular}, shininess{shininess}, reflection{reflection}, color{color} {

	}

	Material::Material(const int &r, const int &g, const int &b) : 
			ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.5}, color{r, g, b} {
		
	}

}
