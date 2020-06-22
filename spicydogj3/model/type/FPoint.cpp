#include "stdafx.h"
#include "FPoint.h"

#include "type.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

FPoint::FPoint() {
}

FPoint::FPoint(double _x,double _y) {
	x = _x;
	y = _y;
}

FPoint::FPoint(const FPoint& fpt) {
	x = fpt.x;
	y = fpt.y;
}

FPoint::FPoint(const POINT& pt) {
	x = pt.x;
	y = pt.y;
}

FPoint::FPoint(const CSize& sz) {
	x = sz.cx;
	y = sz.cy;
}

FPoint FPoint::operator+(const FPoint& fpt) const {
	return FPoint(x + fpt.x,y+fpt.y);
}

FPoint FPoint::operator-(const FPoint& rhs) const {
	return FPoint(x - rhs.x,y - rhs.y);
}

FPoint::operator CPoint() const {
	return CPoint(double_2_int(x),double_2_int(y));
}

BOOL FPoint::operator ==(const FPoint& rhs)const {
	return rhs.x == x && rhs.y == y;
}
