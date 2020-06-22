#include "stdafx.h"
#include "FSize.h"

#include "type.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

FSize::FSize() {
}

FSize::FSize(double _cx,double _cy) {
	cx = _cx;
	cy = _cy;
}

FSize::FSize(const FSize& fsz) {
	cx = fsz.cx;
	cy = fsz.cy;
}

FSize::FSize(const SIZE& sz) {
	cx = sz.cx;
	cy = sz.cy;
}

FSize::operator CSize() const {
	return CSize(double_2_int(cx),double_2_int(cy));
}

BOOL FSize::operator ==(const FSize& rhs)const {
	return rhs.cx == cx && rhs.cy == cy;
}
