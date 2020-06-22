#pragma once

#include "sdom/SDOMSerializable.h"

class Placing;
class SDOMArchive;

/**	�A�j���[�V������� */
class Animating : public SDOMSerializable {
public:
	BOOL animating;
	int  easing;

	Animating();
	virtual void Serialize(SDOMArchive*);
	void Animate(Placing*,const Placing* l,const Placing* r,int n,int len);

	static BOOL IsValidEasing(int);
};
