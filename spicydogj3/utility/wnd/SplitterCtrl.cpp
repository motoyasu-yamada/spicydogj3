#include "stdafx.h"
#include "SplitterCtrl.h"

#include "utility/CursorCapture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



const int DRAG_RECT_SIZE = 8;

BEGIN_MESSAGE_MAP(SplitterCtrl, CStatic)
	//{{AFX_MSG_MAP(SplitterCtrl)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
SplitterCtrl::SplitterCtrl(UINT nWmMsg,HCURSOR hcur)
:	m_nWmMsg(nWmMsg),
	m_hcur(hcur),
	m_nPane1MinSize(32),
	m_nPane2MinSize(32) {
}

SplitterCtrl::~SplitterCtrl() {
}

void SplitterCtrl::SetPane1MinSize(UINT n) {
	m_nPane1MinSize = n;
}

void SplitterCtrl::SetPane2MinSize(UINT n) {
	m_nPane2MinSize = n;
}

BOOL SplitterCtrl::OnSetCursor(CWnd*, UINT, UINT)  {
	ASSERT(m_hcur != NULL);

    VERIFY(::SetCursor(m_hcur));
    return TRUE;
}

void SplitterCtrl::NotifySplitterMove() {
	GetParent()->SendMessage(m_nWmMsg,GetSplitterPos(),GetDlgCtrlID());
}

void SplitterCtrl::GetParentClientRect(LPRECT prc) const {
	GetWindowRect(prc);
	GetParent()->ScreenToClient(prc);
}

void SplitterCtrl::OnPaint() {
	CPaintDC dc(this);
}

BOOL SplitterCtrl::OnEraseBkgnd(CDC* pDC) {
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc,::GetSysColor(COLOR_BTNFACE));
	return TRUE;
}

void SplitterCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UNUSED_ALWAYS(point);
	UNUSED_ALWAYS(nFlags);
	if (::GetCapture() != NULL)
		return;
	
	if (DoSplitterMove())
		NotifySplitterMove();
}

class CClientDCEx : public CDC {
	DECLARE_DYNAMIC(CClientDCEx)
protected:
	HWND m_hWnd;
public:
	CClientDCEx(CWnd* pWnd,CRgn* prgnClip = NULL,DWORD dw = DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
	virtual ~CClientDCEx();
}; // class CClientDCEx

IMPLEMENT_DYNAMIC(CClientDCEx,CDC)
CClientDCEx::CClientDCEx(CWnd* pWnd,CRgn* prgnClip,DWORD flags) : 
	m_hWnd(NULL) {
	ASSERT(pWnd == NULL || ::IsWindow(pWnd->m_hWnd));
	m_hWnd = pWnd->GetSafeHwnd();
	if (!Attach(pWnd->GetDCEx(prgnClip,flags)->GetSafeHdc()))
		AfxThrowResourceException();
}

CClientDCEx::~CClientDCEx() {
	ASSERT(m_hDC != NULL);
	::ReleaseDC(m_hWnd, Detach());
}

void OffsetChildWindow(CWnd* pWnd,CPoint ptOffset) {
	ASSERT_VALID(pWnd);
	ASSERT(pWnd->GetStyle() & WS_CHILD);

	CRect rc;
	pWnd->GetClientRect(rc);
	pWnd->ClientToScreen(rc);
	CWnd* pParent = pWnd->GetParent();
	ASSERT_VALID(pParent);
	pParent->ScreenToClient(rc);

	CPoint ptTo = rc.TopLeft() + ptOffset;
	pWnd->SetWindowPos(NULL,ptTo.x,ptTo.y,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

CPoint GetCursorClientPos(CWnd* pWnd) {
	ASSERT_VALID(pWnd);
	CPoint pt;
	VERIFY(GetCursorPos(&pt));
	pWnd->ScreenToClient(&pt);
	return pt;
}

BOOL SplitterCtrl::DoSplitterMove()  {
	CRect rcClip;
	GetTrackClipRect(rcClip);

	CursorClipper clip(rcClip);
	CursorCapture cap(*this);

	GetDragSplitRect(m_rcDragOrg);
	m_rcDragCur = m_rcDragOrg;
	m_ptDragOrg = m_ptDragCur = GetCursorClientPos(this);
	m_szDragBorder  = m_rcDragCur.Size();

	CClientDCEx dc(GetParent());

	dc.DrawDragRect(m_rcDragCur,m_szDragBorder,NULL,m_szDragBorder);
		BOOL bResult = Track(dc);	
	dc.DrawDragRect(m_rcDragCur,m_szDragBorder,NULL,m_szDragBorder);
	
	if (bResult) {
		CPoint pt = m_ptDragCur-m_ptDragOrg;
		int h = GetPosFromPoint(pt);
		pt = CPoint(0,0);
		GetPosFromPoint(pt) = h;
		OffsetChildWindow(this,pt);
	}
	return bResult;
}

void SplitterCtrl::GetPointFromLPARAM(CPoint& pt,LPARAM lparam) {
	pt.x = (SHORT) LOWORD(lparam);
	pt.y = (SHORT) HIWORD(lparam);
}

BOOL SplitterCtrl::Track(CDC& dc) {
	BOOL bResult = FALSE;
	::AfxLockTempMaps();

	CRect rcOld;
	int nDiff;
	for (;;) {
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != this)
			goto exit;

		switch (msg.message) {
		case WM_LBUTTONUP:	
			bResult = TRUE;
			goto exit;
		case WM_MOUSEMOVE:
			GetPointFromLPARAM(m_ptDragCur,msg.lParam);
			nDiff = GetPosFromPoint(m_ptDragCur) - GetPosFromPoint(m_ptDragOrg);
			rcOld = m_rcDragCur;
			m_rcDragCur = m_rcDragOrg;
			GetPosFromPoint(m_rcDragCur.TopLeft())     += nDiff;
			GetPosFromPoint(m_rcDragCur.BottomRight()) += nDiff;
			dc.DrawDragRect(m_rcDragCur,m_szDragBorder,rcOld,m_szDragBorder);
			break;
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				goto exit;
		case WM_RBUTTONDOWN:
			goto exit;
		default:
			DispatchMessage(&msg);
			break;
		}
	}

exit:
	::AfxUnlockTempMaps(FALSE);
	return bResult;
}

UINT SplitterCtrl::GetSplitterPos() const {
	CRect rc;
	GetParentClientRect(rc);
	return GetPosFromPointC(rc.CenterPoint());
}

void SplitterCtrl::GetTrackClipRect(CRect& rc)
{
	GetParent()->GetClientRect(rc);
	GetParent()->ClientToScreen(rc);

	GetPosFromPoint(rc.TopLeft())     += m_nPane1MinSize;
	GetPosFromPoint(rc.BottomRight()) -= m_nPane2MinSize;
}

const LONG& SplitterCtrl::GetPosFromPointC(const CPoint& pt) const {
	SplitterCtrl* pThis = const_cast<SplitterCtrl*>(this);
	return pThis->GetPosFromPoint(const_cast<CPoint&>(pt));
}


SplitterVCtrl::SplitterVCtrl()
:	SplitterCtrl(WM_SAFX_SPLITV_MOVE,
				  AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_SIZENS))) {
}

SplitterVCtrl::~SplitterVCtrl() {
}


LONG& SplitterVCtrl::GetPosFromPoint(CPoint& pt) {
	return pt.y;
}

void SplitterVCtrl::GetDragSplitRect(CRect& rc) const {
	GetParentClientRect(rc);
	int yc = (rc.bottom + rc.top) / 2;
	rc.top    = yc - DRAG_RECT_SIZE / 2;
	rc.bottom = yc + DRAG_RECT_SIZE / 2;
}

SplitterHCtrl::SplitterHCtrl()
:	SplitterCtrl(WM_SAFX_SPLITH_MOVE,
				  AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_SIZEWE)))
{
}

SplitterHCtrl::~SplitterHCtrl() {
}

void SplitterHCtrl::GetDragSplitRect(CRect& rc) const {
	GetParentClientRect(rc);
	int xc = (rc.right + rc.left) / 2;
	rc.left  = xc - DRAG_RECT_SIZE / 2;
	rc.right = xc + DRAG_RECT_SIZE / 2;
}

LONG& SplitterHCtrl::GetPosFromPoint(CPoint& pt) {
	return pt.x;
}

void SplitterHCtrl::SetLeftPaneMinWidth(UINT n) {
	SplitterCtrl::SetPane1MinSize(n);
}

void SplitterHCtrl::SetRightPaneMinWidth(UINT n) {
	SplitterCtrl::SetPane2MinSize(n);
}
