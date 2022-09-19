#pragma once

#include <cassert>
#include <cmath>

#include <SFML/Graphics.hpp>
#include "tools.h"

namespace rtrace {

	/*  Material class
		This class contains all required informations 
		about material properties in render algorith.
		User should use this class in custom objects to 
		define color, texture and other properties.          */

	class Material {
	private:
		double ambient, diffuse, specular, shininess, reflection; //phong model
		sf::Color color;
	public:
		Material();
		Material(const unsigned int&, const unsigned int&, const unsigned int&);

		void setAmbient(const double&);
		void setDiffuse(const double&);
		void setSpecular(const double&);
		void setShininess(const double&);
		void setReflection(const double&);
		void setColor(const sf::Color&);
		void setColor(const unsigned int&, const unsigned int&, const unsigned int&);

		const double & getAmbient() const;
		const double & getDiffuse() const;
		const double & getSpecular() const;
		const double & getShininess() const;
		const double & getReflection() const;
		sf::Color getColor() const;
	};

}
