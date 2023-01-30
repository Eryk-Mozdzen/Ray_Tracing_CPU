#include <rtrace/view.h>

namespace rtrace {

	View::View(const Vector3 &position, const double &distance) : distance{distance} {
		transform.translate(position);
	}

	Vector3 View::getDirection(const Vector3 &dir) const {
		return transform.getRotation()*dir;
	}

	Vector3 View::getPosition() const {
		return transform.getTranslation();
	}

	double View::getDistanceFromProjectionPlane() const {
		return distance;
	}

	void View::setDistanceFromProjectionPlane(const double &distance) {
		this->distance = distance;
	}

	void View::translate(const Vector3 &transltion) {
		transform.translate(transltion);
	}

	void View::rotate(const Vector3 &axis, const double &theta) {
		transform.rotate(axis, theta);
	}

}
