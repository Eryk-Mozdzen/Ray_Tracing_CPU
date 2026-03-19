#ifndef RTRACE_MATERIAL_HPP
#define RTRACE_MATERIAL_HPP

#include "rtrace/Color.hpp"

namespace rtrace {

/*  Material class
    This class contains all required informations
    about material properties in render algorithm.  */

struct Material {
    double ambient, diffuse, specular, shininess, reflection; // Phong model
    Color color;

    Material();
    Material(const Color &);
    Material(const Color &,
             const double &,
             const double &,
             const double &,
             const double &,
             const double &);
    Material(const int &, const int &, const int &);
};

}

#endif
