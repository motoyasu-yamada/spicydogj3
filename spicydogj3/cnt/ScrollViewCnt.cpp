#include "stdafx.h"
#include "resource.h"
#include "ScrollViewCnt.H"
#include "cnt/CntMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ScrollViewZoomCnt,CCmdTarget)
	ON_COMMAND(IDCANCEL,OnCancle)
END_MESSAGE_MAP()

ScrollViewZoomCnt::ScrollViewZoomCnt() {
}

void ScrollViewZoomCnt::OnLButtonDown(UINT,CPoint pt) {
	SetScale(GetViewT()->GetScale() * 2,pt);
}

void ScrollViewZoomCnt::OnRButtonDown(UINT,CPoint pt) {
	SetScale(GetViewT()->GetScale() / 2,pt);
}

void ScrollViewZoomCnt::SetScale(double scale,CPoint pt) {
	CPoint ptLP = pt;
	GetViewT()->DPtoLP(&ptLP);
	GetViewT()->SetScale(scale);
	GetViewT()->ScrollToCenter(ptLP);
}

BOOL ScrollViewZoomCnt::OnSetCursor(CWnd* pWnd,UINT nHit,UINT nMsg) {
	if (nHit == HTCLIENT) {
		SetCursor(AfxGetApp()->LoadCursor(IDC_SCRVIEW_ZOOM));
		return TRUE;
	}
	return GetViewT()->Default();
}

void ScrollViewZoomCnt::OnCancle() {
	GetCntMgr()->ActivateDefaultControler();
}


BEGIN_MESSAGE_MAP(ScrollViewMoveCnt,CCmdTarget)
	ON_COMMAND(IDCANCEL,OnCancle)
END_MESSAGE_MAP()
ScrollViewMoveCnt::ScrollViewMoveCnt() {
}

void ScrollViewMoveCnt::OnLButtonDown(UINT,CPoint pt) {
	m_ptPrev = pt;
}

const int MOVE_SENSIBILITY = 4;

void ScrollViewMoveCnt::OnMouseMove(UINT n,CPoint pt) {
	if (!n & MK_LBUTTON)
		return;

	CSize szOff = - (pt - m_ptPrev);
	const BOOL bBoost = n & MK_SHIFT;
	const int nBoost = bBoost ? 2 : 1;
	szOff.cx *= nBoost;
	szOff.cy *= nBoost;

	if (abs(szOff.cx) < MOVE_SENSIBILITY && 
		abs(szOff.cy) < MOVE_SENSIBILITY) {
		return;
	}

	CPoint ptNew = GetViewT()->GetDeviceScrollPosition();
	ptNew.x += szOff.cx * nBoost;
	ptNew.y += szOff.cy * nBoost;

	ptNew.x = max(0,min(GetViewT()->GetScrollLimit(SB_HORZ),ptNew.x));
	ptNew.y = max(0,min(GetViewT()->GetScrollLimit(SB_VERT),ptNew.y));

	GetViewT()->ScrollToDevicePosition(ptNew);
	m_ptPrev = pt;
}

BOOL ScrollViewMoveCnt::OnSetCursor(CWnd*,UINT nHit,UINT) {
	if (nHit == HTCLIENT) {
		SetCursor(AfxGetApp()->LoadCursor(IDC_SCRVIEW_MOVE));
		return TRUE;
	}

	return GetViewT()->Default();
}

void ScrollViewMoveCnt::OnCancle() {
	GetCntMgr()->ActivateDefaultControler();
}

void ScrollViewMoveCnt::OnRButtonDown(UINT,CPoint) {
	OnCancle();
}
