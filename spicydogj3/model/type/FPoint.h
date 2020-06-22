#pragma once

class FSize;

/**	高精度な相対座標管理クラス */
class FPoint {
public:
	double x;
	double y;

	FPoint();
	FPoint(double x,double y);
	FPoint(const POINT& pt);
	FPoint(const FPoint& fpt);
	FPoint(const CSize&);
	FPoint operator+(const FPoint&) const;
	FPoint operator-(const FPoint&) const;
	FPoint operator-(const FSize&) const;

	operator CPoint() const;
	BOOL operator == (const FPoint&) const;
};
