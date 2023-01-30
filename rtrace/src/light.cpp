#include <rtrace/light.h>

namespace rtrace {

	Light::Light() {}

	Light::Light(const Vector3 &position) {
		this->position = position;
	}

	const Vector3 & Light::getPosition() const {
		return this->position;
	}

}
