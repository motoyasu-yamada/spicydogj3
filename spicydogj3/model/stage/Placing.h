#pragma once

#include "sdom/SDOMSerializable.h"

class AffineMatrix;

/** 配置 */
class Placing : public SDOMSerializable {
public:
	/** 位置 */
	int x;
	/** 位置 */
	int y;
	/** 比率 */
	double cx;
	/** 比率 */
	double cy;
	/** ラジアン */
	double radian;

	Placing();
	Placing& operator=(const Placing&);
	virtual void Serialize(SDOMArchive*);
	void Interpolate(const Placing* left,const Placing* right,int n,int len,int easing);
	void Translate(AffineMatrix*);
};
