#include "stdafx.h"
#include "type.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int double_2_int(double d) {
	// ƒ[ƒ‚ÉŒü‚©‚Á‚ÄŠÛ‚ß‚ç‚ê‚Ü‚·
	if (0 < d) {
		return d + 0.5;
	} else {
		return d - 0.5;
	}
}

double interpolate(int n,int len,int easing,double left,double right) {
	ASSERT(0 <= n && n < len);
	
	if (n == 0) {
		return left;
	}
	double dT = static_cast<double>(n) / len;
	ASSERT(0 < dT && dT < 1);
	double dD = right - left;
	double dN = left + dD * dT;
	
	if (easing == 0) {
		return dN;
	}

	double dE;
	if (0 < easing) {
		// Œ¸‘¬
		dE = left + dD * (2 * dT - dT * dT);

	} else {
		// ‰Á‘¬ 
		ASSERT(easing < 0);
		dE = left + dD * dT * dT;
	}

	int nAbsAccent = abs(easing);
	return (dN * (100 - nAbsAccent) + dE * nAbsAccent) / 100;
}