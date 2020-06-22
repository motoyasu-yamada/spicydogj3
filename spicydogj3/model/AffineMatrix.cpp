#include "stdafx.h"
#include "AffineMatrix.h"

#include "model/type/type.h"
#include "model/type/FSize.h"
#include "model/type/FPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

AffineMatrix::AffineMatrix()
:	cx(1),
	cy(1),
	rx(0),
	ry(0),
	x(0),
	y(0)
{
}

AffineMatrix& AffineMatrix::operator =(const AffineMatrix& rhs) {
	cx = rhs.cx;
	cy = rhs.cy;
	rx = rhs.rx;
	ry = rhs.ry;
	x = rhs.x;
	y = rhs.y;
	return *this;
}

/** ‹t•ÏŠ·ŒöŽ®
	aqz - ary - pbz + pcy + xbr - xcq

	qz - ry	cy - bz	br - cq
	rx - pz az - cz cp - ar
	py - qx bx - ay aq - bp

  	a	b	c	: cx rx	x
	p	q	r	: cy ry	y
	x	y	z	: 0	 0  1

	11 = a 12 = b 13 = c
	21 = p 22 = q 23 = r
	31 = x 32 = y 33 = z
	denominator = _a*(_q*_z-_r*_y) +_b*(_r*_x-_p*_z) +_c*(_p*_y-_q*_x);

	a = (_q*_z - _r*_y) / denominator; 
	b = (_c*_y - _b*_z) / denominator;
	c = (_b*_r - _c*_q) / denominator;
	d = (_r*_x - _p*_z) / denominator;
	e = (_a*_z - _c*_x) / denominator;
	f = (_c*_p - _a*_r) / denominator;
	g = (_p*_y - _q*_x) / denominator;
	h = (_b*_x - _a*_y) / denominator;
	i = (_a*_q - _b*_p) / denominator;

	denominator = _11*(_22*_33-_23*_32) +_12*(_23*_31-_21*_33) +_13*(_21*_32-_22*_31);

	a = (_22*_33 - _23*_32) / denominator; 
	b = (_13*_32 - _12*_33) / denominator;
	c = (_12*_23 - _13*_22) / denominator;
	d = (_23*_31 - _21*_33) / denominator;
	e = (_11*_33 - _13*_31) / denominator;
	f = (_13*_21 - _11*_23) / denominator;
	g = (_21*_32 - _22*_31) / denominator;
	h = (_12*_31 - _11*_32) / denominator;
	i = (_11*_22 - _12*_21) / denominator;
*/
AffineMatrix AffineMatrix::Inverse() const {
	const double& a = cx;
	const double& b = rx;
	const double  c = x;
	const double& p = ry;
	const double& q = cy;
	const double  r = y;
	const double  x = 0;
	const double  y = 0;
	const double  z = 1;

	const double adbc = a*q*z - a*r*y -p*b*z + p*c*y + x*b*r - x*c*q;
	ASSERT(adbc != 0);

	AffineMatrix m;
	m.cx = (q*z - r*y)/adbc;
	m.rx = (c*y - b*z)/adbc;
	m.x  = (b*r - c*q)/adbc;
	m.ry = (r*x - p*z)/adbc;
	m.cy = (a*z - c*x)/adbc;
	m.y  = (c*p - a*r)/adbc;

	return m;
}

void AffineMatrix::Interpolate(const AffineMatrix* left,const AffineMatrix* right,int n,int len,int easing) {
	cx = interpolate(n,len,easing,left->cx,right->cx);
	cy = interpolate(n,len,easing,left->cy,right->cy);

	x  = interpolate(n,len,easing,left->x,right->x);
	y  = interpolate(n,len,easing,left->y,right->y);

	rx = interpolate(n,len,easing,left->rx,right->rx);
	ry = interpolate(n,len,easing,left->ry,right->ry);
}

AffineMatrix operator*(const AffineMatrix& lhs,const AffineMatrix& rhs) {
	AffineMatrix matrix;
	matrix.cx = lhs.cx * rhs.cx + lhs.rx * rhs.ry;
	matrix.rx = lhs.cx * rhs.rx + lhs.rx * rhs.cy;
	matrix.x  = lhs.cx * rhs.x	+ 
				lhs.rx * rhs.y  + 
				lhs.x;
	matrix.ry = lhs.ry * rhs.cx	+ lhs.cy * rhs.ry;
	matrix.cy = lhs.ry * rhs.rx + lhs.cy * rhs.cy;
	matrix.y  = lhs.ry * rhs.x  + 
				lhs.cy * rhs.y  +
				lhs.y;

	return matrix;
}

template <typename _S>
_S multiplyS(const AffineMatrix& l,const _S& r) {
	_S sz;
	sz.cx = l.cx * r.cx + l.rx * r.cy;
	sz.cy = l.cy * r.cy + l.ry * r.cx;

	return sz;
}

template <typename _P>
_P multiplyP(const AffineMatrix& l,const _P& r) {
	_P p;
	p.x = l.cx * r.x + l.rx * r.y + l.x;
	p.y = l.cy * r.y + l.ry * r.x + l.y;

	return p;
}

CSize operator *(const AffineMatrix& lhs,const CSize& rhs) {
	return multiplyS(lhs,rhs);
}

FSize operator *(const AffineMatrix& lhs,const FSize& rhs) {
	return multiplyS(lhs,rhs);
}

CPoint operator* (const AffineMatrix& lhs,const CPoint& rhs) {
	return multiplyP(lhs,rhs);
}

FPoint operator* (const AffineMatrix& lhs,const FPoint& rhs) {
	return multiplyP(lhs,rhs);
}
