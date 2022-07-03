#pragma once

namespace MatVec {

	class Vector4 {
	public:
		Vector4();
		Vector4(double x, double y, double z, double w);
		~Vector4();
		double v[4];
		double& operator()(unsigned int ind);
		Vector4 operator+() const;
		Vector4 operator-() const;
		Vector4& operator*=(double d);
		Vector4& operator/=(double d);
		Vector4& operator+=(const Vector4& v2);
		Vector4& operator-=(const Vector4& v2);
		double Norm();
		double Norm2();
		void Normalize();
	};
	Vector4 operator*(const Vector4& v, double d);
	Vector4 operator*(double d, const Vector4& v);
	Vector4 operator/(const Vector4& v, double d);
	Vector4 operator+(const Vector4& v1, const Vector4& v2);
	Vector4 operator-(const Vector4& v1, const Vector4& v2);
	bool operator==(const Vector4& v1, const Vector4& v2);
	bool operator!=(const Vector4& v1, const Vector4& v2);
	/// <summary>
	/// ì‡êœÇãÅÇﬂÇÈ
	/// </summary>
	inline double Dot(Vector4 v1, Vector4 v2)
	{
		return (v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2] + v1.v[3] * v2.v[3]);
	};

}