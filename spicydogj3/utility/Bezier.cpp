#include "stdafx.h"
#include "Bezier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void GetBezierCircumscriptionRect(const BEZIER& bezier,CRect& rc) {
	int l = bezier[0].x;
	int r = l;
	int t = bezier[0].y;
	int b = t;

	int numCurve = (bezier.GetSize() - 1) / 3;
	const POINT* ppt = bezier.GetData();
	while (0 < numCurve --) {
		function<3> fx;
		function<3> fy;
		GetBezierFunc(fx,fy,ppt);
		ppt += 3;
		for (double _t = 0;_t <= 1;_t += 0.01) {
			double x = fx(_t);
			double y = fy(_t);
			l = min(l,x);
			r = max(r,x);
			t = min(t,y);
			b = max(b,y);
		}
	}
	rc.SetRect(l,t,r,b);
}

void GetCBezierCircumscriptionRect(const CBEZIER& _cb,CRect& rc) {
	CBEZIER& cb = const_cast<CBEZIER&>(_cb);

	rc.SetRectEmpty();
	const int num = cb.GetSize();
	for (int n = 0;n < num;n++) {
		CRect rc1;
		GetBezierCircumscriptionRect(cb[n],rc1);
		rc = rc | rc1;
	}
}

void PolyBeizer(CDC* pDC,const BEZIER& b) {
	pDC->PolyBezier(b.GetData(),b.GetSize());
}

void PolyBezier(CDC* pDC,const CBEZIER& _cb) {
	CBEZIER& cb = const_cast<CBEZIER&>(_cb);
	const int num = cb.GetSize();
	for (int n = 0;n < num;n++) {
		PolyBeizer(pDC,cb[n]);
	}
}

BOOL CreatePathFromCBezier(CDC* pDC,const CBEZIER& cb) {
	if (!pDC->BeginPath()) {
		ASSERT(FALSE);
		return FALSE;
	}

	PolyBezier(pDC,cb);

	if (!pDC->EndPath()) {
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}


BOOL CreateRgnFromBezier(const CBEZIER& cb,CRgn& rgn){
	CClientDC dcDesktop(CWnd::GetDesktopWindow());
	CDC dc;
	if (!dc.CreateCompatibleDC(&dcDesktop)) {
		return FALSE;
	}
	if (!CreatePathFromCBezier(&dc,cb)) {
		return FALSE;
	}
	if (!rgn.CreateFromPath(&dc)) {
		return FALSE;
	}
	return TRUE;
}

BOOL PtInBezier(const CBEZIER& cb,POINT pt) {
	CRgn rgn;
	if (!CreateRgnFromBezier(cb,rgn)) {
		return FALSE;
	}
	CRect rc;
	rgn.GetRgnBox(rc);
	TRACE("(%d,%d)-(%d,%d)=(%d,%d)\n",rc,pt);
	return rgn.PtInRegion(pt);
}


BOOL RectInBezier(const CBEZIER& cb,LPCRECT prc) {
	CRgn rgn;
	if (!CreateRgnFromBezier(cb,rgn)) {
		return FALSE;
	}
	return rgn.RectInRegion(prc);
}

inline POINT MatrixByPoint(
	double sx,double rx,double tx,
	double ry,double sy,double ty,POINT src){
	POINT dst;
	dst.x = sx * src.x + rx * src.y + tx;
	dst.y = ry * src.x + sy * src.y + ty;
	return dst;
}

void MatrixByPoints(
	double sx,double rx,double tx,
	double ry,double sy,double ty,
	LPPOINT ppt,int num) {
	ASSERT(AfxIsValidAddress(ppt,sizeof POINT * num));
	while (0 < num--) {
		*ppt = MatrixByPoint(sx,rx,tx,ry,sy,ty,*ppt);
		ppt++;
	}
}

void MatrixByCBezier(
	double sx,double rx,double tx,
	double ry,double sy,double ty,CBEZIER& cb) {
	int num = cb.GetSize();
	for (int n = 0;n < num;n++) {
		BEZIER& b = cb[n];
		MatrixByPoints(sx,rx,tx,ry,sy,ty,b.GetData(),b.GetSize());
	}
}

/**
	ベジェ曲線をポリゴンに変換
*/
void BezierToPoly() {

}

/// ポリゴンを分割し各ポリゴン情報を16点以下にする


