#include "stdafx.h"
#include "Placing.h"

#include "model/AffineMatrix.h"
#include "model/type/type.h"
#include "sdom/SDOMArchive.h"
#include "sdom/sdomn.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Placing::Placing() {
	x  =  y = 0;
	cx = cy = 1;
	radian  = 0;
}

Placing& Placing::operator=(const Placing& right) {
	x  = right.x;
	y  = right.y;
	cx = right.cx;
	cy = right.cy;
	radian = right.radian;
	return *this;
}

void Placing::Serialize(SDOMArchive* ar) {
	ar->SerializeINT(WSDOMN_X, x);
	ar->SerializeINT(WSDOMN_Y, y);
	ar->SerializeDOUBLE(WSDOMN_CX,cx);
	ar->SerializeDOUBLE(WSDOMN_CY,cy);
	ar->SerializeDOUBLE(WSDOMN_RADIAN,radian);
}

void Placing::Interpolate(const Placing* left,const Placing* right,int n,int len,int easing) {
	x  = interpolate(n,len,easing,left->x,right->x);
	y  = interpolate(n,len,easing,left->y,right->y);
	cx = interpolate(n,len,easing,left->cx,right->cx);
	cy = interpolate(n,len,easing,left->cy,right->cy);
	radian = interpolate(n,len,easing,left->radian,right->radian);
}

void Placing::Translate(AffineMatrix* matrix) {
	AffineMatrix m1;
	m1.cx = cx;
	m1.cy = cy;

	AffineMatrix m2;
	m2.cx =  cos(radian);
	m2.rx = -sin(radian);
	m2.ry =  sin(radian);
	m2.cy =  cos(radian);

	AffineMatrix m3;
	m3.x = x;
	m3.y = y;

	*matrix = m3 * m2 * m1;
}
