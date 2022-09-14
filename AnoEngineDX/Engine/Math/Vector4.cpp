#include "Vector4.h"

namespace MatVec {

	MatVec::Vector4::Vector4()
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
		v[3] = 0;
	}

	MatVec::Vector4::Vector4(double x, double y, double z, double w)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}

	MatVec::Vector4::~Vector4()
	{
	}

	double& MatVec::Vector4::operator()(unsigned int ind)
	{
		return v[ind];
	}

	Vector4 MatVec::Vector4::operator+() const
	{
		return Vector4(v[0], v[1], v[2], v[3]);
	}

	Vector4 MatVec::Vector4::operator-() const
	{
		return Vector4(-v[0], -v[1], -v[2], -v[3]);
	}

	Vector4& MatVec::Vector4::operator*=(double d)
	{
		v[0] *= d;
		v[1] *= d;
		v[2] *= d;
		v[3] *= d;
		return *this;
	}

	Vector4& MatVec::Vector4::operator/=(double d)
	{
		v[0] /= d;
		v[1] /= d;
		v[2] /= d;
		v[3] /= d;
		return *this;
	}

	Vector4& MatVec::Vector4::operator+=(const Vector4& v2)
	{
		v[0] += v2.v[0];
		v[1] += v2.v[1];
		v[2] += v2.v[2];
		v[3] += v2.v[3];
		return *this;
	}

	Vector4& MatVec::Vector4::operator-=(const Vector4& v2)
	{
		v[0] -= v2.v[0];
		v[1] -= v2.v[1];
		v[2] -= v2.v[2];
		v[3] -= v2.v[3];
		return *this;
	}

	double MatVec::Vector4::Norm()
	{
		return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
	}

	double Vector4::Norm2()
	{
		return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] * v[3] * v[3];
	}

	void MatVec::Vector4::Normalize()
	{
		double norm = Norm();
		*this /= norm;
	}

	Vector4 MatVec::operator*(const Vector4& v, double d)
	{
		return Vector4(v.v[0] * d, v.v[1] * d, v.v[2] * d, v.v[3] * d);
	}

	Vector4 MatVec::operator*(double d, const Vector4& v)
	{
		return Vector4(v.v[0] * d, v.v[1] * d, v.v[2] * d, v.v[3] * d);
	}

	Vector4 MatVec::operator/(const Vector4& v, double d)
	{
		return Vector4(v.v[0] / d, v.v[1] / d, v.v[2] / d, v.v[3] / d);
	}

	Vector4 MatVec::operator+(const Vector4& v1, const Vector4& v2)
	{
		return Vector4(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2], v1.v[3] + v2.v[3]);
	}

	Vector4 MatVec::operator-(const Vector4& v1, const Vector4& v2)
	{
		return Vector4(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2], v1.v[3] - v2.v[3]);
	}

	bool MatVec::operator==(const Vector4& v1, const Vector4& v2)
	{
		return (v1.v[0] == v2.v[0] && v1.v[1] == v2.v[1] && v1.v[2] == v2.v[2] && v1.v[3] == v2.v[3]);
	}

	bool MatVec::operator!=(const Vector4& v1, const Vector4& v2)
	{
		return !(v1 == v2);
	}

}