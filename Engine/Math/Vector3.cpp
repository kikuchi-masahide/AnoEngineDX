#include "Vector3.h"

namespace MatVec {

	MatVec::Vector3::Vector3()
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
	}

	MatVec::Vector3::Vector3(double x, double y, double z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	MatVec::Vector3::~Vector3()
	{
	}

	double& MatVec::Vector3::operator()(unsigned int ind)
	{
		return v[ind];
	}

	Vector3 MatVec::Vector3::operator+() const
	{
		return Vector3(v[0], v[1], v[2]);
	}

	Vector3 MatVec::Vector3::operator-() const
	{
		return Vector3(-v[0], -v[1], -v[2]);
	}

	Vector3& MatVec::Vector3::operator*=(double d)
	{
		v[0] *= d;
		v[1] *= d;
		v[2] *= d;
		return *this;
	}

	Vector3& MatVec::Vector3::operator/=(double d)
	{
		v[0] /= d;
		v[1] /= d;
		v[2] /= d;
		return *this;
	}

	Vector3& MatVec::Vector3::operator+=(const Vector3& v2)
	{
		v[0] += v2.v[0];
		v[1] += v2.v[1];
		v[2] += v2.v[2];
		return *this;
	}

	Vector3& MatVec::Vector3::operator-=(const Vector3& v2)
	{
		v[0] -= v2.v[0];
		v[1] -= v2.v[1];
		v[2] -= v2.v[2];
		return *this;
	}

	double MatVec::Vector3::Norm()
	{
		return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}

	double Vector3::Norm2()
	{
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	}

	void MatVec::Vector3::Normalize()
	{
		double norm = Norm();
		*this /= norm;
	}

	Vector3 MatVec::operator*(const Vector3& v, double d)
	{
		return Vector3(v.v[0] * d, v.v[1] * d, v.v[2] * d);
	}

	Vector3 MatVec::operator*(double d, const Vector3& v)
	{
		return Vector3(v.v[0] * d, v.v[1] * d, v.v[2] * d);
	}

	Vector3 MatVec::operator/(const Vector3& v, double d)
	{
		return Vector3(v.v[0] / d, v.v[1] / d, v.v[2] / d);
	}

	Vector3 MatVec::operator+(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2]);
	}

	Vector3 MatVec::operator-(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2]);
	}

	bool MatVec::operator==(const Vector3& v1, const Vector3& v2)
	{
		return (v1.v[0] == v2.v[0] && v1.v[1] == v2.v[1] && v1.v[2] == v2.v[2]);
	}

	bool MatVec::operator!=(const Vector3& v1, const Vector3& v2)
	{
		return !(v1 == v2);
	}

}