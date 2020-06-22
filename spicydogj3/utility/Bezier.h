#pragma once
#include <AfxTempl.h>
#include "utility/Func.h"

/** 単一のベジェ曲線 */
typedef CArray<POINT,POINT&> BEZIER;
/** 複数のベジェ曲線 */
typedef CArray<BEZIER,BEZIER&> CBEZIER;

/** ベジェ曲線を構成する頂点および制御点データを取得する */
inline LPPOINT GetBezierPointData(BEZIER& b) { b.GetData(); } 
/** ベジェ曲線に外接する矩形を求める */
void GetBezierCircumscriptionRect(const BEZIER&,CRect&);
/** 複合ベジェ曲線に外接する矩形を求める */
void GetCBezierCircumscriptionRect(const CBEZIER&,CRect&);
/** 指定された点がリージョンの内側にあるかどうかを調べます */
BOOL PtInBezier(const CBEZIER& cb,POINT pt);
/** 指定された矩形がベジェ曲線パスと重なるかどうかを調べます */
BOOL RectInBezier(const CBEZIER& cb,LPCRECT prc);
/** ベジェ曲線をアフィン変換します */
void MatrixByCBezier(double sx,double rx,double tx,double ry,double sy,double ty,CBEZIER& cb);
/** ベジェ曲線を描画します */
void PolyBezier(CDC* pDC,const CBEZIER& _cb);
/** ベジェ曲線をあらわす関数を取得する */
template <typename _PTT>
void GetBezierFunc(function<3>& fX,function<3>& fY,const _PTT* apt) {
	ASSERT(AfxIsValidAddress(&apt,(sizeof _PTT) * 4,TRUE));
	double x0 = apt[0].x;
	double x1 = apt[1].x;
	double x2 = apt[2].x;
	double x3 = apt[3].x;

	double y0 = apt[0].y;
	double y1 = apt[1].y;
	double y2 = apt[2].y;
	double y3 = apt[3].y;

	fX[0] =  1 * x0;
	fX[1] = -3 * x0 + 3 * x1;
	fX[2] =  3 * x0 - 6 * x1  + 3 * x2;
	fX[3] = -1 * x0 + 3 * x1  - 3 * x2 + 1 * x3;
	
	fY[0] =  1 * y0;
	fY[1] = -3 * y0 + 3 * y1;
	fY[2] =  3 * y0 - 6 * y1  + 3 * y2;
	fY[3] = -1 * y0 + 3 * y1  - 3 * y2 + 1 * y3;
}

