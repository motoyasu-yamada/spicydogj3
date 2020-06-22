#include "stdafx.h"
#include "BitmapStatic.h"
#include <afxpriv.h>
#include "utility/PxLib.h"
#include "utility/PaintMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(BitmapStatic, CStatic)
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE_VOID(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()

BitmapStatic::BitmapStatic()
:	m_bHover(FALSE),
	m_bDown(FALSE)
{
}

BitmapStatic::~BitmapStatic()
{
}

BOOL BitmapStatic::Create(UINT nCurID,UINT nBmpID,CWnd* pParent,UINT nID)
{
	CRect rc;
	rc.SetRectEmpty();
	if (!CStatic::Create(NULL,SS_BITMAP | SS_NOTIFY | WS_VISIBLE | WS_CHILD,rc,pParent,nID))
		return FALSE;
	Setup(nCurID,nBmpID);
	return TRUE;
}	

void BitmapStatic::Setup(UINT nCurID,UINT nBmpID) {
	m_nCurID = nCurID;

	CBitmap bmp;
	VERIFY(bmp.LoadBitmap(nBmpID));
	BITMAP bm;
	VERIFY(bmp.GetBitmap(&bm));

	VERIFY(m_imagelist.Create(bm.bmWidth,bm.bmHeight,ILC_COLOR24,0,10));
	m_imagelist.Add(&bmp,(CBitmap*)NULL);

	SetWindowPos(NULL,0,0,bm.bmWidth,bm.bmHeight,SWP_NOMOVE | SWP_NOZORDER);
}

BOOL BitmapStatic::HitTest(CPoint pt) const
{
	CRect rc;
	GetClientRect(rc);
	return rc.PtInRect(pt);
}

void BitmapStatic::OnPaint() {
	PaintMemDC dc(this);

	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc,::GetSysColor(COLOR_3DFACE));
	dc.DrawEdge(rc,BDR_RAISEDINNER,BF_RECT);

	if (IsWindowEnabled()) {
		m_imagelist.Draw(&dc,0,rc.TopLeft() + (m_bDown ? CPoint(1,1) : CPoint(0,0)),ILD_NORMAL);

		if (m_bHover)
		{
			dc.Draw3dRect(rc,::GetSysColor(COLOR_BTNHILIGHT),::GetSysColor(COLOR_3DDKSHADOW));
			rc.DeflateRect(1,1);
			dc.Draw3dRect(rc,::GetSysColor(COLOR_3DLIGHT),::GetSysColor(COLOR_BTNSHADOW));
		}
		else if (m_bDown)
		{
			dc.Draw3dRect(rc,::GetSysColor(COLOR_3DDKSHADOW),::GetSysColor(COLOR_BTNHILIGHT));
			rc.DeflateRect(1,1);
			dc.Draw3dRect(rc,::GetSysColor(COLOR_BTNSHADOW),::GetSysColor(COLOR_3DLIGHT));
		}
	} else {
		PxLib::DrawEmbossed(dc,m_imagelist,0,rc.TopLeft());
	}
}

BOOL BitmapStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor(AfxGetApp()->LoadCursor(m_nCurID));

	return TRUE;
}

void BitmapStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((!IsWindowEnabled()) || !HitTest(point) || m_bDown)
		return;
	
	CRect rc;
	GetClientRect(rc);

	m_bDown = TRUE;
	SetCapture();
	Invalidate();
	
	::AfxLockTempMaps();
	MSG msg;
	BOOL bCanceled = FALSE;
	for(;;) {
		VERIFY(::GetMessage(&msg,NULL,0,0));

		if (CWnd::GetCapture() != this) {
			bCanceled = TRUE;
			goto exit;
		}
		switch (msg.message) {
		case WM_MOUSEMOVE:
			Invalidate();
			break;
		case WM_LBUTTONUP:
			goto exit;
		case WM_KEYDOWN:
			if (VK_ESCAPE == msg.wParam)
			{
				bCanceled = TRUE;
				goto exit;
			}
		default:
			DispatchMessage(&msg);
		}
	}
exit:
	VERIFY(ReleaseCapture());
	::AfxUnlockTempMaps(FALSE);
	m_bDown = FALSE;
	Invalidate();

	if (!bCanceled)
	{
		CPoint pt;
		VERIFY(GetCursorPos(&pt));
		ScreenToClient(&pt);
		if (HitTest(pt))
			GetParent()->SendMessage(WM_COMMAND,GetDlgCtrlID());
	}
}

void BitmapStatic::OnMouseLeave()
{
	if (m_bHover)
	{
		m_bHover = FALSE;
		Invalidate();
	}
}

inline BOOL CompareBOOL(BOOL r,BOOL l) {
	if (r && l) {
		return TRUE;
	}
	if (!r && !l) {
		return TRUE;
	}
	return FALSE;
}

void BitmapStatic::OnMouseMove(UINT,CPoint point) {
	BOOL bOldHover = m_bHover;
	BOOL bNewHover = HitTest(point);
	if (!CompareBOOL(bOldHover,bNewHover)) {
		m_bHover = bNewHover;
		Invalidate();
	}

	if (!bOldHover)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwHoverTime = 0;
		tme.hwndTrack   = m_hWnd;
		tme.dwFlags     = TME_LEAVE;
		VERIFY(_TrackMouseEvent(&tme));
	}
}

void BitmapStatic::PreSubclassWindow() 
{
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	
	CStatic::PreSubclassWindow();
}
