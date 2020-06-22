#pragma once

#include "FSize.h"
#include "FPoint.h"

class FRect {
public:
	double left;
	double top;
	double right;
	double bottom;

	FRect();
	FRect(double l,double t,double r,double b);
	FRect(LPCRECT);

	operator RECT() const;
	operator LPCRECT() const;

	double Width() const;
	double Height() const;
	FSize Size() const;

	FPoint& TopLeft();
	FPoint& BottomRight();
	const FPoint& TopLeft() const;
	const FPoint& BottomRight() const;
	FPoint CenterPoint() const;
};
