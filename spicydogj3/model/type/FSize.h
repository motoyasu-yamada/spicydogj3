#pragma once

class FPoint;

/**	高精度な相対座標管理クラス */
class FSize {
public:
	double cx;
	double cy;

	FSize();
	FSize(double cx,double cy);
	FSize(const FSize& fsz);
	FSize(const SIZE& sz);
	FSize(const FPoint& fpt);

	operator CSize() const;
	BOOL operator == (const FSize&) const;
};

