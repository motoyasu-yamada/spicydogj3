#pragma once

#include "sdom/SDOMSerializable.h"

class AffineMatrix;

/** �z�u */
class Placing : public SDOMSerializable {
public:
	/** �ʒu */
	int x;
	/** �ʒu */
	int y;
	/** �䗦 */
	double cx;
	/** �䗦 */
	double cy;
	/** ���W�A�� */
	double radian;

	Placing();
	Placing& operator=(const Placing&);
	virtual void Serialize(SDOMArchive*);
	void Interpolate(const Placing* left,const Placing* right,int n,int len,int easing);
	void Translate(AffineMatrix*);
};
