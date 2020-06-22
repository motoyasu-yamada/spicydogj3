#include "stdafx.h"
#include "FRect.h"

#include "type.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




FRect::FRect() {
}

FRect::FRect(double l,double t,double r,double b) {
	left = l;
	top = t;
	right = r;
	bottom = b;
}

FRect::FRect(LPCRECT prc) {
	ASSERT(AfxIsValidAddress(prc,sizeof RECT));

	left = prc->left;
	top = prc->top;
	right = prc->right;
	bottom = prc->bottom;
}

FRect::operator RECT () const {
	RECT rc;
	VERIFY(SetRect(&rc,left,top,right,bottom));
	return rc;
}

double FRect::Width() const {
	return right - left;
}

double FRect::Height() const {
	return bottom - top;
}

FSize FRect::Size() const {
	return FSize(Width(),Height());
}

FPoint& FRect::TopLeft() {
	return *reinterpret_cast<FPoint*>(this); 
}

FPoint& FRect::BottomRight() {
	return *(reinterpret_cast<FPoint*>(this) + 1); 
}

const FPoint& FRect::TopLeft() const {
	return *reinterpret_cast<const FPoint*>(this); 
}

const FPoint& FRect::BottomRight() const {
	return *(reinterpret_cast<const FPoint*>(this) + 1); 
}

FPoint FRect::CenterPoint() const {
	return FPoint(right+left/2,bottom+top/2);
}
