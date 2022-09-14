#pragma once

namespace MatVec {

	class Matrix4x4 {
	public:
		Matrix4x4();
		~Matrix4x4();
		double m[4][4];
		double& operator()(unsigned int i, unsigned int j);
		Matrix4x4 operator+();
		Matrix4x4 operator-();
		Matrix4x4& operator*=(double d);
		Matrix4x4& operator/=(double d);
		Matrix4x4& operator+=(const Matrix4x4& v2);
		Matrix4x4& operator-=(const Matrix4x4& v2);
	};
	Matrix4x4 operator*(const Matrix4x4& v, const double& d);
	Matrix4x4 operator*(double d, const Matrix4x4& v);
	Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);
	Matrix4x4 operator/(const Matrix4x4& v, double d);
	Matrix4x4 operator+(const Matrix4x4& v1, const Matrix4x4& v2);
	Matrix4x4 operator-(const Matrix4x4& v1, const Matrix4x4& v2);
	bool operator==(const Matrix4x4& v1, const Matrix4x4& v2);
	bool operator!=(const Matrix4x4& v1, const Matrix4x4& v2);
	/// <summary>
	/// 4x4単位行列
	/// </summary>
	Matrix4x4 Identity4x4();
	/// <summary>
	/// LU分解で逆行列を求める
	/// </summary>
	Matrix4x4 GetInverseMatrix(Matrix4x4& mat);
	/// <summary>
	/// 原点を中心とする正射影プロジェクション行列を作る
	/// </summary>
	/// <param name="width">近接平面の横幅[px]</param>
	/// <param name="height">近接平面の縦幅[px]</param>
	/// <param name="near_z">近接平面のz座標(変換後0となるz座標)</param>
	/// <param name="far_z">遠方平面のz座標(変換後1となるz座標)</param>
	Matrix4x4 GetOrthoGraphicProjection(double width, double height, double near_z, double far_z);

}