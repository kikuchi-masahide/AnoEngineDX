#pragma once

namespace MatVec
{
	class Vector2 {
	public:
		Vector2();
		Vector2(double x, double y);
		~Vector2();
		double v[2];
		double& operator()(unsigned int ind);
		Vector2 operator+() const;
		Vector2 operator-() const;
		Vector2& operator*=(double d);
		Vector2& operator/=(double d);
		Vector2& operator+=(const Vector2& v2);
		Vector2& operator-=(const Vector2& v2);
		double Norm();
		double Norm2();
		void Normalize();
	};
	Vector2 operator*(const Vector2& v, double d);
	Vector2 operator*(double d, const Vector2& v);
	Vector2 operator/(const Vector2& v, double d);
	Vector2 operator+(const Vector2& v1, const Vector2& v2);
	Vector2 operator-(const Vector2& v1, const Vector2& v2);
	bool operator==(const Vector2& v1, const Vector2& v2);
	bool operator!=(const Vector2& v1, const Vector2& v2);
	/// <summary>
	/// 内積を求める
	/// </summary>
	inline double Dot(Vector2 v1, Vector2 v2)
	{
		return (v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1]);
	}
	/// <summary>
	/// 外積を求める
	/// </summary>
	inline double Cross(Vector2 v1, Vector2 v2)
	{
		return v1(0) * v2(1) - v1(1) * v2(0);
	}
	/// <summary>
	/// x軸となす角がangleである，長さ1のVector2を返す
	/// </summary>
	inline Vector2 DirectionVector(double angle)
	{
		return Vector2(cos(angle), sin(angle));
	}
}