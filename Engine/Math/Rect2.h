#pragma once
#include "MatVec.h"

struct Rect2 {
private:
	MatVec::Vector2 mLD, mRU;
public:
	Rect2();
	Rect2(MatVec::Vector2 _ld, MatVec::Vector2 _ru);
	Rect2(double lx,double rx,double dy,double uy);
	MatVec::Vector2 GetLD() const;
	MatVec::Vector2 GetRU() const;
	MatVec::Vector2& LD();
	MatVec::Vector2& RU();
	double GetWidth();
	double GetHeight();
	MatVec::Vector2 GetCenter();
};