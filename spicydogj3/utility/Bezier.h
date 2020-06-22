#pragma once
#include <AfxTempl.h>
#include "utility/Func.h"

/** �P��̃x�W�F�Ȑ� */
typedef CArray<POINT,POINT&> BEZIER;
/** �����̃x�W�F�Ȑ� */
typedef CArray<BEZIER,BEZIER&> CBEZIER;

/** �x�W�F�Ȑ����\�����钸�_����ѐ���_�f�[�^���擾���� */
inline LPPOINT GetBezierPointData(BEZIER& b) { b.GetData(); } 
/** �x�W�F�Ȑ��ɊO�ڂ����`�����߂� */
void GetBezierCircumscriptionRect(const BEZIER&,CRect&);
/** �����x�W�F�Ȑ��ɊO�ڂ����`�����߂� */
void GetCBezierCircumscriptionRect(const CBEZIER&,CRect&);
/** �w�肳�ꂽ�_�����[�W�����̓����ɂ��邩�ǂ����𒲂ׂ܂� */
BOOL PtInBezier(const CBEZIER& cb,POINT pt);
/** �w�肳�ꂽ��`���x�W�F�Ȑ��p�X�Əd�Ȃ邩�ǂ����𒲂ׂ܂� */
BOOL RectInBezier(const CBEZIER& cb,LPCRECT prc);
/** �x�W�F�Ȑ����A�t�B���ϊ����܂� */
void MatrixByCBezier(double sx,double rx,double tx,double ry,double sy,double ty,CBEZIER& cb);
/** �x�W�F�Ȑ���`�悵�܂� */
void PolyBezier(CDC* pDC,const CBEZIER& _cb);
/** �x�W�F�Ȑ�������킷�֐����擾���� */
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

