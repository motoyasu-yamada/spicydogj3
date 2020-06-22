#include "stdafx.h"
#include "ActTableCtrl.h"
#include "utility/PaintMemDC.h"
#include "utility/wnd/ColorBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int COLORS_OF_ROW = 16;
const int COLORS_OF_COL = 256 / COLORS_OF_ROW;
const UINT IDTOOLTIP_COLOR = 1;

BEGIN_MESSAGE_MAP(ActTableCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0,OnNeedText)
END_MESSAGE_MAP()

ActTableCtrl::ActTableCtrl()
:	m_bHasAct(FALSE),
	m_nToolTipFormat(-1),
	m_bHasTrans(FALSE) {
}

ActTableCtrl::~ActTableCtrl() {
}

void ActTableCtrl::SetToolTipFormat(UINT nId) {
	m_nToolTipFormat = nId;
}

void ActTableCtrl::Init() {
	RecalcLayout();

	if (!m_tooltip.Create(this))
		AfxThrowResourceException();

	CRect rc;
	GetClientRect(rc);
	if (!m_tooltip.AddTool(this,LPSTR_TEXTCALLBACK,rc,IDTOOLTIP_COLOR))
		AfxThrowResourceException();

	m_tooltip.Activate(TRUE);

	if (!EnableToolTips(TRUE))
		AfxThrowResourceException();

	ASSERT(m_tooltip.GetOwner() == this);
}

int ActTableCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Init();
	return 0;
}

void ActTableCtrl::SetAct(const Act* pSrc) {
	m_bHasAct = TRUE;
	m_actCopied = *pSrc;
	EnableScrollBarCtrl(SB_VERT,TRUE);
	Invalidate();
}

void ActTableCtrl::ResetAct() {
	m_bHasAct = FALSE;
	EnableScrollBarCtrl(SB_VERT,FALSE);
	Invalidate();
}

void ActTableCtrl::SetTransColor(BOOL b,COLORREF cr) {
	ASSERT(m_bHasAct);
	m_bHasTrans = b;
	if (m_bHasTrans) {
		m_crTrans = cr;
		ASSERT(0x000000 <= m_crTrans && m_crTrans <= 0xFFFFFF);
	}
	Invalidate();
}

void ActTableCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {	
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;

	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	OnScroll(MAKEWORD(-1, nSBCode), nPos,TRUE);
}

BOOL ActTableCtrl::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) {
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;
	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= m_line;
		break;
	case SB_LINEDOWN:
		y += m_line;
		break;
	case SB_PAGEUP:
		y -= m_page;
		break;
	case SB_PAGEDOWN:
		y += m_page;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}

	BOOL bResult = OnScrollBy(CSize(0, y - yOrig), bDoScroll);
	if (bResult && bDoScroll) {
		UpdateWindow();
	}
	return bResult;
}

BOOL ActTableCtrl::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) {
	int xOrig, x;
	int yOrig, y;

	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL))) {
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL))) {
		sizeScroll.cx = 0;
	}

	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll) {
		ScrollWindow(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
			SetScrollPos(SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos(SB_VERT, y);
	}
	Invalidate();
	return TRUE;
}

CPoint ActTableCtrl::GetDeviceScrollPosition() const {
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);
	return pt;
}

BOOL ActTableCtrl::OnEraseBkgnd(CDC*) {
	return TRUE;
}

void ActTableCtrl::OnPrepareDC(CDC& dc) {
	ASSERT(dc.GetWindowOrg() == CPoint(0,0));
	CPoint pt =- GetDeviceScrollPosition();
	pt.x *= m_cell;
	pt.y *= m_cell;
	//pt.Offset(-1,-1);
	dc.SetViewportOrg(pt);
}

void ActTableCtrl::OnPaint() {
	PaintMemDC dc(this);
	
	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc,::GetSysColor(COLOR_APPWORKSPACE));
	OnPrepareDC(dc);

	CBrush brGrid(RGB(0x00,0x00,0x00));
	const int num = m_bHasAct ? m_actCopied.GetCount() : 0;
	for (int n = 0;n < 256 && n < num;n++) {
		const int x = (n % COLORS_OF_ROW) * m_cell;
		const int y = (n / COLORS_OF_ROW) * m_cell;
		CRect rcCell(x,y,x+m_cell,y+m_cell);

		const COLORREF crCell = m_actCopied.GetColor(n); 
		dc.FillSolidRect(rcCell,crCell);
		if (m_bHasTrans && m_crTrans == crCell) {
			CRect rcTrans = rcCell;
			rcTrans.OffsetRect(-2,-2);
			const int sz = NULLCOLOR_PATTERN_SIZE * 2;
			rcTrans.left = rcTrans.right  - sz;
			rcTrans.top  = rcTrans.bottom - sz;
			FillNullRect(&dc,rcTrans);
			rcTrans.InflateRect(1,1,1,1);
			dc.FrameRect(rcTrans,&brGrid);
		}
		rcCell.InflateRect(0,0,1,1);
		dc.FrameRect(rcCell,&brGrid);
	}
	
	dc.Flip();
}

void ActTableCtrl::RecalcLayout() {
	ShowScrollBar(SB_VERT,TRUE);
	EnableScrollBarCtrl(SB_VERT,FALSE);
	
	CRect rcW;
	GetWindowRect(rcW);
	CRect rc;
	GetClientRect(rc);
	const int cx = rc.Width();
	const int cy = rc.Height();
	m_cell  = cx   / COLORS_OF_ROW;

	SCROLLINFO si = {sizeof SCROLLINFO};
	VERIFY(GetScrollInfo(SB_VERT,&si));

	const int hNeed = m_cell * COLORS_OF_COL;
	BOOL bNeedHBar = cy <= hNeed;
	if (bNeedHBar) {
		m_page = cy;
		m_line = m_page / 10;
		m_line = 0 < m_line ? m_line : 1; 

		si.nMin  = 0;
		si.nMax  = COLORS_OF_COL - 2;
		si.nPage = m_page / m_cell;
		ASSERT(si.nPage <= si.nMax);
		ASSERT(1 <= si.nMax);
		si.nPos  = si.nTrackPos = 0;
	} else {
		si.fMask = SIF_DISABLENOSCROLL;
	}

	VERIFY(SetScrollInfo(SB_VERT,&si,TRUE));

	const int wcx = COLORS_OF_ROW * m_cell + rcW.Width() - cx;
	const int wcy = ((cy + m_cell - 1)/ m_cell) * m_cell + rcW.Height() - cy;
	SetWindowPos(NULL,0,0,wcx,wcy,SWP_NOMOVE | SWP_NOACTIVATE);
}

void ActTableCtrl::OnMouseMove(UINT nFlags, CPoint point) {	
	CWnd::OnMouseMove(nFlags, point);

	const int x = (point.x / m_cell) * m_cell;
	const int y = (point.y / m_cell) * m_cell;
	CRect rcNew(x,y,x+m_cell,y+m_cell);
	m_tooltip.SetToolRect(this,IDTOOLTIP_COLOR,rcNew);	
}

BOOL ActTableCtrl::OnNeedText(UINT id,NMHDR * pNMHDR,LRESULT * pResult) {
	if (!m_bHasAct)
		return FALSE;
	
	if (m_nToolTipFormat == (UINT)-1)
		return FALSE;

	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	CWindowDC dc(this);
	OnPrepareDC(dc);
	dc.DPtoLP(&point);

	const int index = (point.x / m_cell) + COLORS_OF_ROW * (point.y / m_cell);
	if (index < 0 || m_actCopied.GetCount() <= index)
		return FALSE;
	const COLORREF cr = m_actCopied.GetColor(index);
	m_strTooltip.Format(m_nToolTipFormat,index,GetRValue(cr),GetGValue(cr),GetBValue(cr));
	pTTT->lpszText = (LPTSTR)static_cast<LPCTSTR>(m_strTooltip);

	return TRUE;
}

BOOL ActTableCtrl::PreTranslateMessage(MSG* pMsg) {
	m_tooltip.RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}

void DDX_ActTableCtrl(CDataExchange* pDX, int nIDC,ActTableCtrl& ctrl) {
	if (ctrl.m_hWnd != NULL)
		return;

	ASSERT(!pDX->m_bSaveAndValidate);

	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	CWnd* pDlg = pDX->m_pDlgWnd;
	ASSERT_VALID(pDlg);
	CRect rc;
	::GetWindowRect(hWndCtrl,rc);
	pDlg->ScreenToClient(rc);
	VERIFY(::DestroyWindow(hWndCtrl));

	if (!ctrl.CreateEx(WS_EX_STATICEDGE,NULL,NULL,WS_CHILD | WS_BORDER,rc,pDlg,nIDC)) {
		ASSERT(FALSE);
		AfxThrowNotSupportedException();
	}
	ctrl.ShowWindow(SW_SHOW);
	ctrl.UpdateWindow();
}