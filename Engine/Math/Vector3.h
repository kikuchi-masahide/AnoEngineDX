#pragma once

namespace MatVec
{

	class Vector3 {
	public:
		Vector3();
		Vector3(double x, double y, double z);
		~Vector3();
		double v[3];
		double& operator()(unsigned int ind);
		Vector3 operator+() const;
		Vector3 operator-() const;
		Vector3& operator*=(double d);
		Vector3& operator/=(double d);
		Vector3& operator+=(const Vector3& v2);
		Vector3& operator-=(const Vector3& v2);
		double Norm();
		double Norm2();
		void Normalize();
	};
	Vector3 operator*(const Vector3& v, double d);
	Vector3 operator*(double d, const Vector3& v);
	Vector3 operator/(const Vector3& v, double d);
	Vector3 operator+(const Vector3& v1, const Vector3& v2);
	Vector3 operator-(const Vector3& v1, const Vector3& v2);
	bool operator==(const Vector3& v1, const Vector3& v2);
	bool operator!=(const Vector3& v1, const Vector3& v2);
	/// <summary>
	/// ì‡êœÇãÅÇﬂÇÈ
	/// </summary>
	inline double Dot(Vector3 v1, Vector3 v2)
	{
		return (v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2]);
	}

}