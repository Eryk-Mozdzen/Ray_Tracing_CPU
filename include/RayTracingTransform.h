#ifndef RAY_TRACING_TRANSFORM_H
#define RAY_TRACING_TRANSFORM_H

#include <string>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../include/Vector.h"
#include "../include/Matrix.h"
#include "../include/geometry.h"

class Transform3 : public Matrix {
public:
    Transform3();
    Transform3 & operator=(const Matrix &);

    void translate(const Vector3 &d);
    void rotate(const Vector3&, const double&);

    Vector3 getRelativeToTransform(const Vector3&) const;
    Vector3 getRelativeToReferenceFrame(const Vector3&) const;

    Vector3 getTranslation() const;
    Matrix getRotation() const;
};

#endif
