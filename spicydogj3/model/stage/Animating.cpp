#include "stdafx.h"
#include "Animating.h"

#include "Placing.h"
#include "Coloring.h"
#include "model/type/type.h"

#include "sdom/sdomn.h"
#include "sdom/SDOMArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Animating::Animating() {
	animating = false;
	easing    = 0;
}

void Animating::Serialize(SDOMArchive* ar) {
	ASSERT(AfxIsValidAddress(ar,sizeof SDOMArchive));
	ar->SerializeBOOL(WSDOMN_USE_ANIMATION,animating);
	ar->SerializeINT(WSDOMN_EASING,       easing);
}

void Animating::Animate(Placing* placing,const Placing* left,const Placing* right,int n,int len) {
	if (animating) {
		placing->x = interpolate(n,len,easing,left->x,right->x);
		placing->y = interpolate(n,len,easing,left->y,right->y);
	}
}

BOOL Animating::IsValidEasing(int easing) {
	return -100 <= easing && easing <= 100;
}
