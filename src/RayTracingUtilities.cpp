#include "../include/RayTracingUtilities.h"

Ray::Ray() {}

Ray::Ray(const Vector3 &origin, const Vector3 &direction) : Line(origin, direction) {}

bool Ray::intersect(const Vector3 &point, double *t) const {
    double d = distance(Line(this->point, this->direction), point);
    if(d>EPSILON)
        return false;

    *t = direction*(point-this->point);

    return true;
}

Transform3::Transform3() : Matrix(4, 4) {
    *this = Matrix::Identity(4);

    this->invReady = false;
}

Transform3 & Transform3::operator=(const Matrix &rhs) {
    if(rhs.getRows()!=4) throw std::string("Can't assign matrix to transform");
    if(rhs.getCols()!=4) throw std::string("Can't assign matrix to transform");

    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            (*this)(i, j) = rhs(i, j);

    this->invReady = false;

    return *this;
}

void Transform3::translate(const Vector3 &d) {
    Transform3 transform;

    transform(0, 3) = d.x;
    transform(1, 3) = d.y;
    transform(2, 3) = d.z;

    this->invReady = false;

    *this = (*this)*transform;
}

void Transform3::rotate(const Vector3 &axis, const double &theta) {
    Transform3 transform;

    Matrix m(3, 3);
    m(0, 1) = -axis.z;
    m(0, 2) = axis.y;
    m(1, 0) = axis.z;
    m(1, 2) = -axis.x;
    m(2, 0) = -axis.y;
    m(2, 1) = axis.x;

    Matrix rot = m*std::sin(theta) + (Matrix::Identity(3) - axis*axis.transposition())*std::cos(theta) + (axis*axis.transposition());

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            transform(i, j) = rot(i, j);

    this->invReady = false;

    *this = (*this)*transform;
}

Vector3 Transform3::getRelativeToTransform(const Vector3 &v) {
    Matrix p(4, 1);
    p(0, 0) = v.x;
    p(1, 0) = v.y;
    p(2, 0) = v.z;
    p(3, 0) = 1;

    Matrix rel = this->getInverse()*p;

    return Vector3(
        rel(0, 0),
        rel(1, 0),
        rel(2, 0)
    );
}

Vector3 Transform3::getRelativeToReferenceFrame(const Vector3 &v) {
    Matrix p(4, 1);
    p(0, 0) = v.x;
    p(1, 0) = v.y;
    p(2, 0) = v.z;
    p(3, 0) = 1;

    Matrix rel = (*this)*p;

    return Vector3(
        rel(0, 0),
        rel(1, 0),
        rel(2, 0)
    );
}

Vector3 Transform3::getTranslation() const {
    Vector3 result;
    result.x = (*this)(0, 3);
    result.y = (*this)(1, 3);
    result.z = (*this)(2, 3);
    return result;
}

Matrix Transform3::getRotation() const {
    Matrix result(3, 3);
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            result(i, j) = (*this)(i, j);
    return result;
}

Matrix Transform3::getInverse() {
    if(!this->invReady) {
        this->invReady = true;
        this->inv = this->inverse();
    }

    return this->inv;
}

CollisionData::CollisionData() {}

TextureMenager::TextureMenager() {}

void TextureMenager::load(const std::string &texture_str) {
    sf::Image texture;
    texture.loadFromFile(texture_str);
    textures.push_back(texture);
}

sf::Image* TextureMenager::getTextureReference(const int &index) {
    return &textures[index];
}

sf::Color color_interpolation(const sf::Color &color1, const sf::Color &color2, const double &alpha) {
    if(color1==sf::Color::Transparent) return color2;
    if(color2==sf::Color::Transparent) return color1;

    return sf::Color(
        (1.f-alpha)*color1.r + alpha*color2.r,
        (1.f-alpha)*color1.g + alpha*color2.g,
        (1.f-alpha)*color1.b + alpha*color2.b
    );
}




