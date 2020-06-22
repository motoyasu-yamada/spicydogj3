#pragma once

class FPoint;

/**	�����x�ȑ��΍��W�Ǘ��N���X */
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

