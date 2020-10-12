#pragma once

struct Rect2 {
private:
	Vector2 mLD, mRU;
public:
	Rect2();
	Rect2(Vector2 _ld, Vector2 _ru);
	Rect2(double lx,double rx,double dy,double uy);
};