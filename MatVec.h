#pragma once

namespace MatVec
{
	using namespace Eigen;
	using Matrix3x3 = Matrix3d;
	using Matrix4x4 = Matrix4d;
	using Vector2 = Vector2d;
	/// <summary>
	/// 右手系縦3次元ベクトル
	/// </summary>
	using Vector3 = Vector3d;
	/// <summary>
	/// 縦4次元ベクトル
	/// </summary>
	using Vector4 = Vector4d;
	/// <summary>
	/// (x,y,z)を(x,y,z,1)に拡張
	/// </summary>
	Vector4 XYZ1(Vector3 _vec3);
	/// <summary>
	/// (x,y,z)を(x,y,z,0)に拡張
	/// </summary>
	Vector4 XYZ0(Vector3 _vec3);
	/// <summary>
	/// (x,y,z,w)を(x,y,z)に変換
	/// </summary>
	Vector3 XYZ(Vector4 _vec4);
	/// <summary>
	/// (x,y)を(x,y,0)に変換
	/// </summary>
	Vector3 XY0(Vector2 _vec2);
	/// <summary>
	/// 3x3単位行列
	/// </summary>
	Matrix3x3 Identity3x3();
	/// <summary>
	/// 4x4単位行列
	/// </summary>
	Matrix4x4 Identity4x4();
	/// <summary>
	/// (_x,_y,_z)だけ平行移動行列
	/// </summary>
	Matrix4x4 Translation(double _x, double _y,double _z);
	/// <summary>
	/// _vecだけ平行移動する行列
	/// </summary>
	Matrix4x4 Translation(Vector3 _vec);
	/// <summary>
	/// x座標を_x，y座標を_y，z座標を_z倍に拡大する行列
	/// </summary>
	Matrix4x4 Expand(double _x, double _y, double _z);
	/// <summary>
	/// x座標を_vec[0]，y座標を_vec[1]，z座標を_vec[2]倍に拡大する行列
	/// </summary>
	Matrix4x4 Expand(Vector3 _vec);
	/// <summary>
	/// quaternionは4次元ベクトルを流用
	/// </summary>
	using Quaternion = Vector4d;
	/// <summary>
	/// 回転軸と角度を指定してquaternionを取得
	/// </summary>
	Quaternion GetQuaternion(Vector3 _axis, double _theta);
	/// <summary>
	/// quaternionを回転行列に変換
	/// </summary>
	Matrix4x4 Rotate(Quaternion _q);
	/// <summary>
	/// _q1と_q2をt:(1-t)で補完
	/// </summary>
	Quaternion Slerp(Quaternion _q1, Quaternion _q2, double t);
}