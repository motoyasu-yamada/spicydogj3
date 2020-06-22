#pragma once

class FSize;
class FPoint;

/** アフィン変換行列 */
class AffineMatrix {
public:
	double x;
	double y;
	double cx;
	double cy;
	double rx;
	double ry;

	AffineMatrix();
	AffineMatrix& operator=(const AffineMatrix&);
	AffineMatrix Inverse() const;
	void Interpolate(const AffineMatrix*,const AffineMatrix*,int n,int len,int easing);
};

AffineMatrix operator *(const AffineMatrix&,const AffineMatrix&);
FSize operator *(const AffineMatrix&,const FSize&);
FPoint operator *(const AffineMatrix&,const FPoint&);
CSize operator *(const AffineMatrix&,const CSize&);
CPoint operator *(const AffineMatrix&,const CPoint&);

