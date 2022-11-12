#include "Vector2.h"

namespace MatVec {

	MatVec::Vector2::Vector2()
	{
		v[0] = 0;
		v[1] = 0;
	}

	MatVec::Vector2::Vector2(double x, double y)
	{
		v[0] = x;
		v[1] = y;
	}

	MatVec::Vector2::~Vector2()
	{
	}

	double& MatVec::Vector2::operator()(unsigned int ind)
	{
		return v[ind];
	}

	Vector2 MatVec::Vector2::operator+() const
	{
		return Vector2(v[0], v[1]);
	}

	Vector2 MatVec::Vector2::operator-() const
	{
		return Vector2(-v[0], -v[1]);
	}

	Vector2& MatVec::Vector2::operator*=(double d)
	{
		v[0] *= d;
		v[1] *= d;
		return *this;
	}

	Vector2& MatVec::Vector2::operator/=(double d)
	{
		v[0] /= d;
		v[1] /= d;
		return *this;
	}

	Vector2& MatVec::Vector2::operator+=(const Vector2& v2)
	{
		v[0] += v2.v[0];
		v[1] += v2.v[1];
		return *this;
	}

	Vector2& MatVec::Vector2::operator-=(const Vector2& v2)
	{
		v[0] -= v2.v[0];
		v[1] -= v2.v[1];
		return *this;
	}

	double MatVec::Vector2::Norm()
	{
		return sqrt(v[0] * v[0] + v[1] * v[1]);
	}

	double Vector2::Norm2()
	{
		return v[0] * v[0] + v[1] * v[1];
	}

	void MatVec::Vector2::Normalize()
	{
		double norm = Norm();
		*this /= norm;
	}

	Vector2 MatVec::operator*(const Vector2& v, double d)
	{
		return Vector2(v.v[0] * d, v.v[1] * d);
	}

	Vector2 MatVec::operator*(double d, const Vector2& v)
	{
		return Vector2(v.v[0] * d, v.v[1] * d);
	}

	Vector2 MatVec::operator/(const Vector2& v, double d)
	{
		return Vector2(v.v[0] / d, v.v[1] / d);
	}

	Vector2 MatVec::operator+(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.v[0] + v2.v[0], v1.v[1] + v2.v[1]);
	}

	Vector2 MatVec::operator-(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.v[0] - v2.v[0], v1.v[1] - v2.v[1]);
	}

	bool MatVec::operator==(const Vector2& v1, const Vector2& v2)
	{
		return (v1.v[0] == v2.v[0] && v1.v[1] == v2.v[1]);
	}

	bool MatVec::operator!=(const Vector2& v1, const Vector2& v2)
	{
		return !(v1 == v2);
	}

}