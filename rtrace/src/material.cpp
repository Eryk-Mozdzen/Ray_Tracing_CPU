#include <rtrace/material.h>

rtrace::Material::Material() : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{255, 0, 0} {
	
}

rtrace::Material::Material(const int &r, const int &g, const int &b) : 
		ambient{0.3}, diffuse{0.2}, specular{0.2}, shininess{5}, reflection{0.1}, color{r, g, b} {
	
}
