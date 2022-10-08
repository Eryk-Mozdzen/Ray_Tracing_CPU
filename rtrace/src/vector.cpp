#include <rtrace/vector.h>

const rtrace::Vector3 rtrace::Vector3::X(1, 0, 0);
const rtrace::Vector3 rtrace::Vector3::Y(0, 1, 0);
const rtrace::Vector3 rtrace::Vector3::Z(0, 0, 1);

rtrace::Vector3::Vector3() : x{0}, y{0}, z{0} {

}

rtrace::Vector3::Vector3(const double &x, const double &y, const double &z) : x{x}, y{y}, z{z} {

}

rtrace::Vector3 & rtrace::Vector3::operator+=(const rtrace::Vector3 &rhs) {
    x +=rhs.x;
    y +=rhs.y;
    z +=rhs.z;

    return *this;
}

rtrace::Vector3 & rtrace::Vector3::operator-=(const rtrace::Vector3 &rhs) {
    x -=rhs.x;
    y -=rhs.y;
    z -=rhs.z;

    return *this;
}

rtrace::Vector3 rtrace::Vector3::operator+(const rtrace::Vector3 &rhs) const {
    return Vector3(
        x + rhs.x,
        y + rhs.y,
        z + rhs.z
    );
}

rtrace::Vector3 rtrace::Vector3::operator-(const rtrace::Vector3 &rhs) const {
    return Vector3(
        x - rhs.x,
        y - rhs.y,
        z - rhs.z
    );
}

double rtrace::Vector3::operator*(const rtrace::Vector3 &rhs) const {
    return x*rhs.x + y*rhs.y + z*rhs.z;
}

rtrace::Vector3 rtrace::Vector3::operator^(const rtrace::Vector3 &rhs) const {
    return Vector3(
        y*rhs.z - z*rhs.y,
        -(x*rhs.z - z*rhs.x),
        x*rhs.y - y*rhs.x
    );
}

rtrace::Vector3 rtrace::Vector3::operator*(const double &rhs) const {
    return Vector3(
        x*rhs,
        y*rhs,
        z*rhs
    );
}

rtrace::Vector3 rtrace::Vector3::operator/(const double &rhs) const {
    return Vector3(
        x/rhs,
        y/rhs,
        z/rhs
    );
}

rtrace::Vector3 rtrace::operator*(const double &lhs, const rtrace::Vector3 &rhs) {
    return Vector3(
        lhs*rhs.x,
        lhs*rhs.y,
        lhs*rhs.z
    );
}

rtrace::Vector3 rtrace::operator-(const rtrace::Vector3 &rhs) {
	return rtrace::Vector3(
		-rhs.x, 
		-rhs.y, 
		-rhs.z
	);
}

std::ostream & rtrace::operator<<(std::ostream &lhs, const rtrace::Vector3 &rhs) {
    lhs << "[";
    lhs << rhs.x << ", ";
    lhs << rhs.y << ", ";
    lhs << rhs.z;
    lhs << "]";
    return lhs;
}

double rtrace::length(const rtrace::Vector3 &vec) {
    return std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

rtrace::Vector3 rtrace::normalize(const rtrace::Vector3 &vec) {
    const double len = rtrace::length(vec);

    if(len<0.0001)
        return rtrace::Vector3();

    return rtrace::Vector3(
        vec.x/len,
        vec.y/len,
        vec.z/len
    );
}
