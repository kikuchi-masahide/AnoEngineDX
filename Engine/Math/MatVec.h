#pragma once

#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace MatVec
{
	
	Vector4 operator*(const Matrix4x4& m, const Vector4& v);

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
	/// (w=0ならそのまま，w\neq 0ならば/w)
	/// </summary>
	Vector3 XYZ(Vector4 _vec4);
	/// <summary>
	/// (x,y)を(x,y,0)に変換
	/// </summary>
	Vector3 XY0(Vector2 _vec2);
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
	using Quaternion = Vector4;
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
	XMMATRIX ConvertToXMMATRIX(Matrix4x4 mat);
	XMFLOAT2 ConvertToXMFLOAT2(Vector2 vec2);
	XMFLOAT3 ConvertToXMFLOAT3(Vector3 vec3);
}