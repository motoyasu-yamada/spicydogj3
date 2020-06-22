#include "stdafx.h"
#include "Coloring.h"

#include "model/type/type.h"
#include "sdom/sdomn.h"
#include "sdom/SDOMArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Coloring::Coloring() {
	mulRed = mulGreen = mulBlue = 1;
	addRed = addGreen = addBlue = 0;
	editType = EDIT_TYPE_NONE;
}

Coloring::Coloring(const Coloring& right) {
	CopyFrom(right);
}

Coloring& Coloring::operator =(const Coloring& right) {
	CopyFrom(right);
	return *this;
}

void Coloring::CopyFrom(const Coloring& right) {
	mulRed   = right.mulRed;
	mulGreen = right.mulGreen;
	mulBlue  = right.mulBlue;

	addRed   = right.addRed;
	addGreen = right.addGreen;
	addBlue  = right.addBlue;
}

void Coloring::Interpolate(const Coloring* left,const Coloring* right,int n,int len,int easing) {
	mulRed   = interpolate(n,len,easing,left->mulRed,  right->mulRed);
	mulGreen = interpolate(n,len,easing,left->mulGreen,right->mulGreen);
	mulBlue  = interpolate(n,len,easing,left->mulBlue, right->mulBlue);
	addRed   = interpolate(n,len,easing,left->addRed,  right->addRed);
	addGreen = interpolate(n,len,easing,left->addGreen,right->addGreen);
	addBlue  = interpolate(n,len,easing,left->addBlue, right->addBlue);
}

void Coloring::Serialize(SDOMArchive* ar) {
	ar->SerializeINT(WSDOMN_ADD_RED,  addRed);
	ar->SerializeINT(WSDOMN_ADD_GREEN,addGreen);
	ar->SerializeINT(WSDOMN_ADD_BLUE, addBlue);
	ar->SerializeDOUBLE(WSDOMN_MUL_RED,  mulRed);
	ar->SerializeDOUBLE(WSDOMN_MUL_GREEN,mulGreen);
	ar->SerializeDOUBLE(WSDOMN_MUL_BLUE, mulBlue);
}

BOOL Coloring::IsValidAdd(int add) {
	return -255 <= add && add <= 255; 
}

BOOL Coloring::IsValidMul(double mul) {
	return -255 <= mul && mul <= 255;
}

void Coloring::AssertValid() const {
	ASSERT(IsValidAdd(addRed));
	ASSERT(IsValidAdd(addGreen));
	ASSERT(IsValidAdd(addBlue));
	ASSERT(IsValidMul(mulRed));
	ASSERT(IsValidMul(mulGreen));
	ASSERT(IsValidMul(mulBlue));
}

int saturate_color(int n) {
	if (n < 0) {
		return 0;
	}
	if (255 < n) {
		return 255;
	}
	return n;
}

COLORREF operator*(const Coloring& lhs,const COLORREF& rhs) {
	int r,g,b;
	r = saturate_color(lhs.mulRed   * GetRValue(rhs) + lhs.addRed); 
	g = saturate_color(lhs.mulGreen * GetGValue(rhs) + lhs.addGreen); 
	b = saturate_color(lhs.mulBlue  * GetBValue(rhs) + lhs.addBlue); 
	return RGB(r,g,b);
}
