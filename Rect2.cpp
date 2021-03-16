#include "Rect2.h"

Rect2::Rect2():mLD(),mRU() {}

Rect2::Rect2(MatVec::Vector2 _ld, MatVec::Vector2 _ru) :mLD(_ld),mRU(_ru) {}

Rect2::Rect2(double lx, double rx, double dy, double uy)
{
	mLD(0) = lx;
	mLD(1) = dy;
	mRU(0) = rx;
	mRU(1) = uy;
}

MatVec::Vector2 Rect2::GetLD()
{
	return mLD;
}

MatVec::Vector2 Rect2::GetRU()
{
	return mRU;
}

double Rect2::GetWidth()
{
	return mRU(0) - mLD(0);
}

double Rect2::GetHeight()
{
	return mRU(1) - mLD(1);
}

MatVec::Vector2 Rect2::GetCenter()
{
	return (mLD + mRU) / 2;
}
