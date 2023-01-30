#include <rtrace/matrix.h>
#include <iomanip>

namespace rtrace {

	Matrix33::Matrix33() : data{0} {

	}

	Matrix33 Matrix33::operator*(const Matrix33 &mat) const {
		Matrix33 result;

		result(0, 0) = (*this)(0, 0)*mat(0, 0) + (*this)(0, 1)*mat(1, 0) + (*this)(0, 2)*mat(2, 0);
		result(0, 1) = (*this)(0, 0)*mat(0, 1) + (*this)(0, 1)*mat(1, 1) + (*this)(0, 2)*mat(2, 1);
		result(0, 2) = (*this)(0, 0)*mat(0, 2) + (*this)(0, 1)*mat(1, 2) + (*this)(0, 2)*mat(2, 2);

		result(1, 0) = (*this)(1, 0)*mat(0, 0) + (*this)(1, 1)*mat(1, 0) + (*this)(1, 2)*mat(2, 0);
		result(1, 1) = (*this)(1, 0)*mat(0, 1) + (*this)(1, 1)*mat(1, 1) + (*this)(1, 2)*mat(2, 1);
		result(1, 2) = (*this)(1, 0)*mat(0, 2) + (*this)(1, 1)*mat(1, 2) + (*this)(1, 2)*mat(2, 2);

		result(2, 0) = (*this)(2, 0)*mat(0, 0) + (*this)(2, 1)*mat(1, 0) + (*this)(2, 2)*mat(2, 0);
		result(2, 1) = (*this)(2, 0)*mat(0, 1) + (*this)(2, 1)*mat(1, 1) + (*this)(2, 2)*mat(2, 1);
		result(2, 2) = (*this)(2, 0)*mat(0, 2) + (*this)(2, 1)*mat(1, 2) + (*this)(2, 2)*mat(2, 2);

		return result;
	}

	const double & Matrix33::operator()(int row, int col) const {
		return data[3*row + col];
	}

	double & Matrix33::operator()(int row, int col) {
		return data[3*row + col];
	}

	std::ostream& operator<<(std::ostream &stream, const Matrix33 &mat) {
		stream.precision(5);
		stream.setf(std::ios_base::fixed);

		stream << "[" << std::endl;
		stream << "[" << mat(0, 0) << " " << mat(0, 1) << " " << mat(0, 2) << "]," << std::endl;
		stream << "[" << mat(1, 0) << " " << mat(1, 1) << " " << mat(1, 2) << "]," << std::endl;
		stream << "[" << mat(2, 0) << " " << mat(2, 1) << " " << mat(2, 2) << "]" << std::endl;
		stream << "]" << std::endl;

		return stream;
	}

	double determinant(const Matrix33 &mat) {
		return 
			mat(0, 0)*mat(1, 1)*mat(2, 2) + 
			mat(0, 1)*mat(1, 2)*mat(2, 0) + 
			mat(0, 2)*mat(1, 0)*mat(2, 1) - 
			mat(0, 2)*mat(1, 1)*mat(2, 0) - 
			mat(0, 1)*mat(1, 0)*mat(2, 2) - 
			mat(0, 0)*mat(1, 2)*mat(2, 1);
	}

	Matrix33 transposition(const Matrix33 &mat) {
		Matrix33 result = mat;

		result(0, 1) = mat(1, 0);
		result(0, 2) = mat(2, 0);
		result(1, 2) = mat(2, 1);

		result(1, 0) = mat(0, 1);
		result(2, 0) = mat(0, 2);
		result(2, 1) = mat(1, 2);

		return result;
	}

}
