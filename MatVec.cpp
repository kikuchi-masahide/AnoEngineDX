#include "MatVec.h"

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
	return Vector3(_vec4(0),_vec4(1),_vec4(2));
}

MatVec::Vector3 MatVec::XY0(MatVec::Vector2 _vec2)
{
	return Vector3(_vec2(0),_vec2(1),0);
}

MatVec::Matrix3x3 MatVec::Identity3x3()
{
	auto mat = MatVec::Matrix3x3();
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat(i, j) = 0;
		}
	}
	mat(0, 0) = 1;
	mat(1, 1) = 1;
	mat(2, 2) = 1;
	return mat;
}

MatVec::Matrix4x4 MatVec::Identity4x4()
{
	auto mat = MatVec::Matrix4x4();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat(i, j) = 0;
		}
	}
	mat(0, 0) = 1;
	mat(1, 1) = 1;
	mat(2, 2) = 1;
	mat(3, 3) = 1;
	return mat;
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
	if (Zero(_axis.norm()))
	{
		Log::OutputCritical("Zero Vector is set as an axis of quaternion");
	}
	_axis.normalize();
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
	//_q1‚Æ_q2‚ÌŠÔ‚ÌŠp“x
	double omega = acos(_q1.dot(_q2));
	return sin((1 - t) * omega) / sin(omega) * _q1 + sin(t * omega) / sin(omega) * _q2;
}
