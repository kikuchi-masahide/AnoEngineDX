#include "Matrix4x4.h"
#include "../Log.h"
#include "Math.h"

namespace MatVec {

	MatVec::Matrix4x4::Matrix4x4()
	{
		std::fill(m[0], m[4], 0);
	}

	MatVec::Matrix4x4::~Matrix4x4()
	{
	}

	double& MatVec::Matrix4x4::operator()(unsigned int i, unsigned int j)
	{
		return m[i][j];
	}

	Matrix4x4 MatVec::Matrix4x4::operator+()
	{
		return *this;
	}

	Matrix4x4 MatVec::Matrix4x4::operator-()
	{
		m[0][0] *= -1;
		m[0][1] *= -1;
		m[0][2] *= -1;
		m[0][3] *= -1;

		m[1][0] *= -1;
		m[1][1] *= -1;
		m[1][2] *= -1;
		m[1][3] *= -1;

		m[2][0] *= -1;
		m[2][1] *= -1;
		m[2][2] *= -1;
		m[2][3] *= -1;

		m[3][0] *= -1;
		m[3][1] *= -1;
		m[3][2] *= -1;
		m[3][3] *= -1;

		return *this;
	}

	Matrix4x4& MatVec::Matrix4x4::operator*=(double d)
	{
		m[0][0] *= d;
		m[0][1] *= d;
		m[0][2] *= d;
		m[0][3] *= d;

		m[1][0] *= d;
		m[1][1] *= d;
		m[1][2] *= d;
		m[1][3] *= d;

		m[2][0] *= d;
		m[2][1] *= d;
		m[2][2] *= d;
		m[2][3] *= d;

		m[3][0] *= d;
		m[3][1] *= d;
		m[3][2] *= d;
		m[3][3] *= d;

		return *this;
	}

	Matrix4x4& MatVec::Matrix4x4::operator/=(double d)
	{
		m[0][0] /= d;
		m[0][1] /= d;
		m[0][2] /= d;
		m[0][3] /= d;

		m[1][0] /= d;
		m[1][1] /= d;
		m[1][2] /= d;
		m[1][3] /= d;

		m[2][0] /= d;
		m[2][1] /= d;
		m[2][2] /= d;
		m[2][3] /= d;

		m[3][0] /= d;
		m[3][1] /= d;
		m[3][2] /= d;
		m[3][3] /= d;

		return *this;
	}

	Matrix4x4& MatVec::Matrix4x4::operator+=(const Matrix4x4& v2)
	{
		m[0][0] += v2.m[0][0];
		m[0][1] += v2.m[0][1];
		m[0][2] += v2.m[0][2];
		m[0][3] += v2.m[0][3];

		m[1][0] += v2.m[1][0];
		m[1][1] += v2.m[1][1];
		m[1][2] += v2.m[1][2];
		m[1][3] += v2.m[1][3];

		m[2][0] += v2.m[2][0];
		m[2][1] += v2.m[2][1];
		m[2][2] += v2.m[2][2];
		m[2][3] += v2.m[2][3];

		m[3][0] += v2.m[3][0];
		m[3][1] += v2.m[3][1];
		m[3][2] += v2.m[3][2];
		m[3][3] += v2.m[3][3];

		return *this;
	}

	Matrix4x4& MatVec::Matrix4x4::operator-=(const Matrix4x4& v2)
	{
		m[0][0] -= v2.m[0][0];
		m[0][1] -= v2.m[0][1];
		m[0][2] -= v2.m[0][2];
		m[0][3] -= v2.m[0][3];

		m[1][0] -= v2.m[1][0];
		m[1][1] -= v2.m[1][1];
		m[1][2] -= v2.m[1][2];
		m[1][3] -= v2.m[1][3];

		m[2][0] -= v2.m[2][0];
		m[2][1] -= v2.m[2][1];
		m[2][2] -= v2.m[2][2];
		m[2][3] -= v2.m[2][3];

		m[3][0] -= v2.m[3][0];
		m[3][1] -= v2.m[3][1];
		m[3][2] -= v2.m[3][2];
		m[3][3] -= v2.m[3][3];

		return *this;
	}

	Matrix4x4 MatVec::operator*(const Matrix4x4& v, const double& d)
	{
		Matrix4x4 v2 = v;
		v2 *= d;
		return v2;
	}

	Matrix4x4 MatVec::operator*(double d, const Matrix4x4& v)
	{
		Matrix4x4 v2 = v;
		v2 *= d;
		return v2;
	}

	Matrix4x4 MatVec::operator*(const Matrix4x4& a, const Matrix4x4& b)
	{
		Matrix4x4 m;
		m.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0];
		m.m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1];
		m.m[0][2] = a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2];
		m.m[0][3] = a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3];

		m.m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0];
		m.m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1];
		m.m[1][2] = a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2];
		m.m[1][3] = a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3];

		m.m[2][0] = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0];
		m.m[2][1] = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1];
		m.m[2][2] = a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2];
		m.m[2][3] = a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3];

		m.m[3][0] = a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0];
		m.m[3][1] = a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1];
		m.m[3][2] = a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2];
		m.m[3][3] = a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3];

		return m;
	}

	Matrix4x4 MatVec::operator/(const Matrix4x4& v, double d)
	{
		Matrix4x4 m = v;
		m /= d;
		return m;
	}

	Matrix4x4 MatVec::operator+(const Matrix4x4& v1, const Matrix4x4& v2)
	{
		Matrix4x4 m;
		m.m[0][0] = v1.m[0][0] + v2.m[0][0];
		m.m[0][1] = v1.m[0][1] + v2.m[0][1];
		m.m[0][2] = v1.m[0][2] + v2.m[0][2];
		m.m[0][3] = v1.m[0][3] + v2.m[0][3];

		m.m[1][0] = v1.m[1][0] + v2.m[1][0];
		m.m[1][1] = v1.m[1][1] + v2.m[1][1];
		m.m[1][2] = v1.m[1][2] + v2.m[1][2];
		m.m[1][3] = v1.m[1][3] + v2.m[1][3];

		m.m[2][0] = v1.m[2][0] + v2.m[2][0];
		m.m[2][1] = v1.m[2][1] + v2.m[2][1];
		m.m[2][2] = v1.m[2][2] + v2.m[2][2];
		m.m[2][3] = v1.m[2][3] + v2.m[2][3];

		m.m[3][0] = v1.m[3][0] + v2.m[3][0];
		m.m[3][1] = v1.m[3][1] + v2.m[3][1];
		m.m[3][2] = v1.m[3][2] + v2.m[3][2];
		m.m[3][3] = v1.m[3][3] + v2.m[3][3];

		return m;
	}

	Matrix4x4 MatVec::operator-(const Matrix4x4& v1, const Matrix4x4& v2)
	{
		Matrix4x4 m;
		m.m[0][0] = v1.m[0][0] - v2.m[0][0];
		m.m[0][1] = v1.m[0][1] - v2.m[0][1];
		m.m[0][2] = v1.m[0][2] - v2.m[0][2];
		m.m[0][3] = v1.m[0][3] - v2.m[0][3];

		m.m[1][0] = v1.m[1][0] - v2.m[1][0];
		m.m[1][1] = v1.m[1][1] - v2.m[1][1];
		m.m[1][2] = v1.m[1][2] - v2.m[1][2];
		m.m[1][3] = v1.m[1][3] - v2.m[1][3];

		m.m[2][0] = v1.m[2][0] - v2.m[2][0];
		m.m[2][1] = v1.m[2][1] - v2.m[2][1];
		m.m[2][2] = v1.m[2][2] - v2.m[2][2];
		m.m[2][3] = v1.m[2][3] - v2.m[2][3];

		m.m[3][0] = v1.m[3][0] - v2.m[3][0];
		m.m[3][1] = v1.m[3][1] - v2.m[3][1];
		m.m[3][2] = v1.m[3][2] - v2.m[3][2];
		m.m[3][3] = v1.m[3][3] - v2.m[3][3];

		return m;
	}

	bool MatVec::operator==(const Matrix4x4& v1, const Matrix4x4& v2)
	{
		return (
			v1.m[0][0] == v2.m[0][0] &&
			v1.m[0][1] == v2.m[0][1] &&
			v1.m[0][2] == v2.m[0][2] &&
			v1.m[0][3] == v2.m[0][3] &&

			v1.m[1][0] == v2.m[1][0] &&
			v1.m[1][1] == v2.m[1][1] &&
			v1.m[1][2] == v2.m[1][2] &&
			v1.m[1][3] == v2.m[1][3] &&

			v1.m[2][0] == v2.m[2][0] &&
			v1.m[2][1] == v2.m[2][1] &&
			v1.m[2][2] == v2.m[2][2] &&
			v1.m[2][3] == v2.m[2][3] &&

			v1.m[3][0] == v2.m[3][0] &&
			v1.m[3][1] == v2.m[3][1] &&
			v1.m[3][2] == v2.m[3][2] &&
			v1.m[3][3] == v2.m[3][3]
			);
	}

	bool MatVec::operator!=(const Matrix4x4& v1, const Matrix4x4& v2)
	{
		return !(v1 == v2);
	}

	MatVec::Matrix4x4 MatVec::Identity4x4()
	{
		auto mat = MatVec::Matrix4x4();
		mat(0, 0) = 1;
		mat(1, 1) = 1;
		mat(2, 2) = 1;
		mat(3, 3) = 1;
		return mat;
	}

	MatVec::Matrix4x4 MatVec::GetInverseMatrix(Matrix4x4& A)
	{
		//LU分解
		Matrix4x4 L = Identity4x4();
		Matrix4x4 U = Identity4x4();
		U.m[0][0] = A.m[0][0];
		U.m[0][1] = A.m[0][1];
		U.m[0][2] = A.m[0][2];
		U.m[0][3] = A.m[0][3];
		L.m[1][0] = A.m[1][0] / U.m[0][0];
		U.m[1][1] = A.m[1][1] - L.m[1][0] * U.m[0][1];
		U.m[1][2] = A.m[1][2] - L.m[1][0] * U.m[0][2];
		U.m[1][3] = A.m[1][3] - L.m[1][0] * U.m[0][3];
		L.m[2][0] = A.m[2][0] / U.m[0][0];
		L.m[2][1] = (A.m[2][1] - L.m[2][0] * U.m[0][1]) / A.m[1][1];
		U.m[2][2] = A.m[2][2] - L.m[2][0] * U.m[0][2] - L.m[2][1] * U.m[1][2];
		U.m[2][3] = A.m[2][3] - L.m[2][0] * U.m[0][3] - L.m[2][1] * U.m[1][3];
		L.m[3][0] = A.m[3][0] / U.m[0][0];
		L.m[3][1] = (A.m[3][1] - L.m[3][0] * U.m[0][1]) / U.m[1][1];
		L.m[3][2] = (A.m[3][2] - L.m[3][0] * U.m[0][2] - L.m[3][1] * U.m[1][2]) / U.m[2][2];
		U.m[3][3] = A.m[3][3] - L.m[3][0] * U.m[0][3] - L.m[3][1] * U.m[1][3] - L.m[3][2] * U.m[2][3];

		Matrix4x4 Y;
		Y.m[0][0] = 1;
		Y.m[1][0] = 0 - L.m[1][0] * Y.m[0][0];
		Y.m[2][0] = 0 - L.m[2][0] * Y.m[0][0] - L.m[2][1] * Y.m[1][0];
		Y.m[3][0] = 0 - L.m[3][0] * Y.m[0][0] - L.m[3][1] * Y.m[1][0] - L.m[3][2] * Y.m[2][0];
		Y.m[0][1] = 0;
		Y.m[1][1] = 1 - L.m[1][0] * Y.m[0][1];
		Y.m[2][1] = 0 - L.m[2][0] * Y.m[0][1] - L.m[2][1] * Y.m[1][1];
		Y.m[3][1] = 0 - L.m[3][0] * Y.m[0][1] - L.m[3][1] * Y.m[1][1] - L.m[3][2] * Y.m[2][1];
		Y.m[0][2] = 0;
		Y.m[1][2] = 0 - L.m[1][0] * Y.m[0][2];
		Y.m[2][2] = 1 - L.m[2][0] * Y.m[0][2] - L.m[2][1] * Y.m[1][2];
		Y.m[3][2] = 0 - L.m[3][0] * Y.m[0][2] - L.m[3][1] * Y.m[1][2] - L.m[3][2] * Y.m[2][2];
		Y.m[0][3] = 0;
		Y.m[1][3] = 0 - L.m[1][0] * Y.m[0][3];
		Y.m[2][3] = 0 - L.m[2][0] * Y.m[0][3] - L.m[2][1] * Y.m[1][3];
		Y.m[3][3] = 1 - L.m[3][0] * Y.m[0][3] - L.m[3][1] * Y.m[1][3] - L.m[3][2] * Y.m[2][3];

		Matrix4x4 X;
		X.m[3][0] = (Y.m[3][0]) / U.m[3][3];
		X.m[2][0] = (Y.m[2][0] - U.m[2][3] * X.m[3][0]) / U.m[2][2];
		X.m[1][0] = (Y.m[1][0] - U.m[1][2] * X.m[2][0] - U.m[1][3] * X.m[3][0]) / U.m[1][1];
		X.m[0][0] = (Y.m[0][0] - U.m[0][1] * X.m[1][0] - U.m[0][2] * X.m[2][0] - U.m[0][3] * X.m[3][0]) / U.m[0][0];
		X.m[3][1] = (Y.m[3][1]) / U.m[3][3];
		X.m[2][1] = (Y.m[2][1] - U.m[2][3] * X.m[3][1]) / U.m[2][2];
		X.m[1][1] = (Y.m[1][1] - U.m[1][2] * X.m[2][1] - U.m[1][3] * X.m[3][1]) / U.m[1][1];
		X.m[0][1] = (Y.m[0][1] - U.m[0][1] * X.m[1][1] - U.m[0][2] * X.m[2][1] - U.m[0][3] * X.m[3][1]) / U.m[0][0];
		X.m[3][2] = (Y.m[3][2]) / U.m[3][3];
		X.m[2][2] = (Y.m[2][2] - U.m[2][3] * X.m[3][2]) / U.m[2][2];
		X.m[1][2] = (Y.m[1][2] - U.m[1][2] * X.m[2][2] - U.m[1][3] * X.m[3][2]) / U.m[1][1];
		X.m[0][2] = (Y.m[0][2] - U.m[0][1] * X.m[1][2] - U.m[0][2] * X.m[2][2] - U.m[0][3] * X.m[3][2]) / U.m[0][0];
		X.m[3][3] = (Y.m[3][3]) / U.m[3][3];
		X.m[2][3] = (Y.m[2][3] - U.m[2][3] * X.m[3][3]) / U.m[2][2];
		X.m[1][3] = (Y.m[1][3] - U.m[1][2] * X.m[2][3] - U.m[1][3] * X.m[3][3]) / U.m[1][1];
		X.m[0][3] = (Y.m[0][3] - U.m[0][1] * X.m[1][3] - U.m[0][2] * X.m[2][3] - U.m[0][3] * X.m[3][3]) / U.m[0][0];

		return X;
	}
	MatVec::Matrix4x4 MatVec::GetOrthoGraphicProjection(double width, double height, double near_z, double far_z)
	{
		BOOST_ASSERT_MSG(!Zero(width), "invalid width given to MatVec::GetOrthoGraphicProjection");
		BOOST_ASSERT_MSG(!Zero(height), "invalid height given to MatVec::GetOrthoGraphicProjection");
		BOOST_ASSERT_MSG(near_z < far_z, "invalid near_z & far_z given to MatVec::GetOrthoGraphicProjection");
		BOOST_ASSERT_MSG(!Zero(far_z - near_z), "invalid near_z & far_z given to MatVec::GetOrthoGraphicProjection");
		Matrix4x4 res = Identity4x4();
		res(0, 0) = 2 / width;
		res(1, 1) = 2 / height;
		res(2, 2) = 1 / (far_z - near_z);
		res(2, 3) = near_z / (near_z - far_z);
		return res;
	}

}