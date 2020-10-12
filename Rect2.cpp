#include "Rect2.h"

Rect2::Rect2():mLD(),mRU() {}

Rect2::Rect2(Vector2 _ld, Vector2 _ru) :mLD(_ld),mRU(_ru) {}

Rect2::Rect2(double lx, double rx, double dy, double uy)
{
	boost::qvm::X(mLD) = lx;
	boost::qvm::Y(mLD) = dy;
	boost::qvm::X(mRU) = rx;
	boost::qvm::Y(mRU) = uy;
}
