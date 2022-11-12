#include "MatVec.h"
#include "MathMacro.h"
#include "../Log.h"

using namespace MatVec;

MatVec::Vector4 MatVec::XYZ1(MatVec::Vector3 _vec3)
{
	return MatVec::Vector4(_vec3(0),_vec3(1),_vec3(2),1);
}

MatVec::Vector4 MatVec::XYZ0(MatVec::Vector3 _vec3)
{
	return MatVec::Vector4(_vec3(0),_vec3(1),_vec3(2),0);
}

MatVec::Vector3 MatVec::XYZ(MatVec::Vector4 _vec4)
{
	if (Zero(_vec4(3)))
	{
		return Vector3(_vec4(0), _vec4(1), _vec4(2));
	}
	return Vector3(_vec4(0) / _vec4(3), _vec4(1) / _vec4(3), _vec4(2) / _vec4(3));
}

MatVec::Vector3 MatVec::XY0(MatVec::Vector2 _vec2)
{
	return Vector3(_vec2(0),_vec2(1),0);
}

MatVec::Matrix4x4 MatVec::Translation(double _x, double _y, double _z)
{
	auto mat = Identity4x4();
	mat(0, 3) = _x;
	mat(1, 3) = _y;
	mat(2, 3) = _z;
	return mat;
}

MatVec::Matrix4x4 MatVec::Translation(Vector3 _vec)
{
	auto mat = Identity4x4();
	mat(0, 3) = _vec(0);
	mat(1, 3) = _vec(1);
	mat(2, 3) = _vec(2);
	return mat;
}

MatVec::Matrix4x4 MatVec::Expand(double _x, double _y, double _z)
{
	auto mat = Matrix4x4();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat(i, j) = 0;
		}
	}
	mat(0, 0) = _x;
	mat(1, 1) = _y;
	mat(2, 2) = _z;
	mat(3, 3) = 1;
	return mat;
}

MatVec::Matrix4x4 MatVec::Expand(Vector3 _vec)
{
	auto mat = Matrix4x4();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat(i, j) = 0;
		}
	}
	mat(0, 0) = _vec(0);
	mat(1, 1) = _vec(1);
	mat(2, 2) = _vec(2);
	mat(3, 3) = 1;
	return mat;
}

MatVec::Quaternion MatVec::GetQuaternion(MatVec::Vector3 _axis, double _theta)
{
	if (Zero(_axis.Norm()))
	{
		Log::OutputCritical("Zero Vector is set as an axis of quaternion");
	}
	_axis.Normalize();
	double x = _axis(0) * sin(_theta / 2);
	double y = _axis(1) * sin(_theta / 2);
	double z = _axis(2) * sin(_theta / 2);
	double w = cos(_theta / 2);
	return Quaternion(x, y, z, w);
}

MatVec::Matrix4x4 MatVec::Rotate(Quaternion _q)
{
	auto mat = Matrix4x4();
	double x = _q(0);
	double y = _q(1);
	double z = _q(2);
	double w = _q(3);

	mat(0, 0) = 1 - 2 * y * y - 2 * z * z;
	mat(0, 1) = 2 * x * y + 2 * z * w;
	mat(0, 2) = 2 * x * z - 2 * w * y;
	mat(0, 3) = 0;

	mat(1, 0) = 2 * x * y - 2 * w * z;
	mat(1, 1) = 1 - 2 * x * x - 2 * z * z;
	mat(1, 2) = 2 * y * z + 2 * w * x;
	mat(1, 3) = 0;

	mat(2, 0) = 2 * x * z + 2 * w * y;
	mat(2, 1) = 2 * y * z - 2 * w * x;
	mat(2, 2) = 1 - 2 * x * x - 2 * y * y;
	mat(2, 3) = 0;

	mat(3, 0) = 0;
	mat(3, 1) = 0;
	mat(3, 2) = 0;
	mat(3, 3) = 1;
	return mat;
}

MatVec::Quaternion MatVec::Slerp(MatVec::Quaternion _q1, MatVec::Quaternion _q2, double t)
{
	double omega = acos(Dot(_q1,_q2));
	return sin((1 - t) * omega) / sin(omega) * _q1 + sin(t * omega) / sin(omega) * _q2;
}

XMMATRIX MatVec::ConvertToXMMATRIX(Matrix4x4 mat)
{
	DirectX::XMMATRIX dxmatrix;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			dxmatrix.r[i].m128_f32[j] = static_cast<float>(mat(i, j));
		}
	}
	return dxmatrix;
}

XMFLOAT2 MatVec::ConvertToXMFLOAT2(Vector2 vec2)
{
	return DirectX::XMFLOAT2(static_cast<float>(vec2(0)),vec2(1));
}

XMFLOAT3 MatVec::ConvertToXMFLOAT3(Vector3 vec3)
{
	return DirectX::XMFLOAT3(vec3(0),vec3(1),vec3(2));
}

Vector4 MatVec::operator*(const Matrix4x4& m, const Vector4& v)
{
	Vector4 res;
	res.v[0] = m.m[0][0] * v.v[0] + m.m[0][1] * v.v[1] + m.m[0][2] * v.v[2] + m.m[0][3] * v.v[3];
	res.v[1] = m.m[1][0] * v.v[0] + m.m[1][1] * v.v[1] + m.m[1][2] * v.v[2] + m.m[1][3] * v.v[3];
	res.v[2] = m.m[2][0] * v.v[0] + m.m[2][1] * v.v[1] + m.m[2][2] * v.v[2] + m.m[2][3] * v.v[3];
	res.v[3] = m.m[3][0] * v.v[0] + m.m[3][1] * v.v[1] + m.m[3][2] * v.v[2] + m.m[3][3] * v.v[3];
	return res;
}


