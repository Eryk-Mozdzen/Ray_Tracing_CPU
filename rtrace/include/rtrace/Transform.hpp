#ifndef RTRACE_TRANSFORM_HPP
#define RTRACE_TRANSFORM_HPP

#include "rtrace/Ray.hpp"

namespace rtrace {

class Transform3 {
    Matrix33 rotation;
    Vector3 translation;

public:
    Transform3();

    void setTranslation(const Vector3 &vec);
    void setRotation(const Vector3 &axis, const double &theta);

    void translate(const Vector3 &vec);
    void rotate(const Vector3 &axis, const double &theta);

    Vector3 convertWordToFrame(const Vector3 &vec) const;
    Vector3 convertFrameToWord(const Vector3 &vec) const;

    Ray convertWordToFrame(const Ray &ray) const;
    Ray convertFrameToWord(const Ray &ray) const;

    const Vector3 &getTranslation() const;
    const Matrix33 &getRotation() const;
};

}

#endif
