#include "vector.h"

Vector2 GetVector2(double x, double y)
{
	Vector2 vec;
	boost::qvm::X(vec) = x;
	boost::qvm::Y(vec) = y;
	return vec;
}

Vector3 GetVector3(double x, double y, double z)
{
	Vector3 vec;
	boost::qvm::X(vec) = x;
	boost::qvm::Y(vec) = y;
	boost::qvm::Z(vec) = z;
	return vec;
}