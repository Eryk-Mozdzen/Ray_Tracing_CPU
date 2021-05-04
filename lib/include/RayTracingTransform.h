#ifndef RAY_TRACING_TRANSFORM_H
#define RAY_TRACING_TRANSFORM_H

#include <string>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Vector.h"
#include "Matrix.h"
#include "RayTracingMath.h"

/*  Transform3 class
    Class represents 3-dimensional transform matrix (4x4) to
    describes translation and rotation of objects in scene.     */

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
