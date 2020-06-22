#include "stdafx.h"
#include "ColorBox.h"
#include "utility/CursorCapture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void FillNullRect(CDC* pdc,LPCRECT prc) 
{
	CRect rc(prc);
	rc.NormalizeRect();

	BOOL bXStartFirst = true;
	for (int nLeft = rc.left;nLeft < rc.right;nLeft += NULLCOLOR_PATTERN_SIZE)
	{
		int nRight = std::_MIN((int)rc.right,nLeft + NULLCOLOR_PATTERN_SIZE);
		
		BOOL bYStartFirst = bXStartFirst;
		for (int nTop = rc.top;nTop < rc.bottom;nTop += NULLCOLOR_PATTERN_SIZE)
		{
			int nBottom = std::_MIN((int)rc.bottom,nTop + NULLCOLOR_PATTERN_SIZE);

			CRect rcDraw(nLeft,nTop,nRight,nBottom);
			if (bYStartFirst)
				pdc->FillSolidRect(rcDraw,NULLCOLOR_1);
			else
				pdc->FillSolidRect(rcDraw,NULLCOLOR_2);

			bYStartFirst = ! bYStartFirst;
		}

		bXStartFirst = !bXStartFirst;
	}
}
void FillSolidRect(CDC* pdc,LPCRECT prc,COLORREF cr)
{
	if (cr == NULLCOLOR)
		FillNullRect(pdc,prc);
	else
		pdc->FillSolidRect(prc,cr);
}


IMPLEMENT_DYNAMIC(ColorBox,CStatic);

BEGIN_MESSAGE_MAP(ColorBox, CStatic)
	//{{AFX_MSG_MAP(ColorBox)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

ColorBox::ColorBox() {
}

ColorBox::~ColorBox() {
}

void ColorBox::SetColor(COLORREF crColor) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBox)));

	if (m_crColor == crColor) {
		return;
	}
	m_crColor = crColor;
	if (::IsWindow(m_hWnd)) {
		Invalidate(FALSE);
	}
}

COLORREF ColorBox::GetColor() const {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBox)));

	return m_crColor;
}

void ColorBox::OnPaint() {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBox)));

	CPaintDC dc(this);
	
	CRect rcClient;
	GetClientRect(&rcClient);

	BOOL bEnabled = IsWindowEnabled();
	COLORREF crFill = bEnabled ? m_crColor : ::GetSysColor(COLOR_GRAYTEXT);
	FillSolidRect(&dc,&rcClient,crFill);

	dc.Draw3dRect(&rcClient,::GetSysColor(COLOR_3DDKSHADOW),::GetSysColor(COLOR_3DHILIGHT));
	rcClient.DeflateRect(1,1);
	dc.Draw3dRect(&rcClient,::GetSysColor(COLOR_3DSHADOW),::GetSysColor(COLOR_3DLIGHT));
}

BOOL ColorBox::OnEraseBkgnd(CDC*) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBox)));
	return TRUE;
}

BOOL ColorBox::Create(const CRect& rc,CWnd* pParent,UINT nID) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBox)));
	return CStatic::Create(NULL,WS_CHILD | WS_VISIBLE,rc,pParent,nID);
}



#define ID_COLORBOX_STATIC	1
#define ID_COLORBOX_BUTTON   2
CFont   f_fontArrow;

IMPLEMENT_DYNAMIC(ColorBoxEx, CWnd);
BEGIN_MESSAGE_MAP(ColorBoxEx, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_ENABLE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0,OnNeedText)
END_MESSAGE_MAP()

ColorBoxEx::ColorBoxEx() 
:	m_idIcon(0),
	m_bDown(FALSE) {
	clinit();
}

void ColorBoxEx::clinit() {
	static BOOL clinited = FALSE;
	if (clinited) {
		return;
	}

	if (!f_fontArrow.CreatePointFont(90,_T("Marlett"))) {
		ASSERT(FALSE);
		AfxThrowResourceException();
	}

	ASSERT(!clinited);
	clinited = TRUE;
}

ColorBoxEx::~ColorBoxEx() {
}

void ColorBoxEx::SetColor(COLORREF crColor) {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBoxEx)));

	if (GetColor() == crColor) {
		return;
	}
	m_cr = crColor;
	if (m_hWnd != NULL) {
		Invalidate();
	}
}

COLORREF ColorBoxEx::GetColor() const {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBoxEx)));
	return m_cr;
}



BOOL ColorBoxEx::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

void ColorBoxEx::OnEnable(BOOL bEnable) {
	Invalidate();
}

void ColorBoxEx::OnKillFocus(CWnd*) {
	Invalidate();
}

void ColorBoxEx::OnSetFocus(CWnd*) {
	Invalidate();
}

CSize GetBitmapSize(UINT n) {
	CBitmap bmp;
	if (!bmp.LoadBitmap(n)) {
		ASSERT(FALSE);
		return CSize(0,0);
	}
	BITMAP bm;
	if (!bmp.GetBitmap(&bm)) {
		ASSERT(FALSE);
		return CSize(0,0);
	}
	return CSize(bm.bmWidth,bm.bmHeight);
}
int ColorBoxEx::OnCreate(LPCREATESTRUCT lpCreateStruct ) {
	ASSERT(m_idIcon != 0);

	CSize szIcn = GetBitmapSize(m_idIcon);
	if (!m_image.Create(m_idIcon,szIcn.cx,1,m_crMask)) {
		ASSERT(FALSE);
		return -1;
	}

	CRect rcClient;
	GetClientRect(rcClient);
	rcClient.DeflateRect(2,2);

	m_rcIcon = rcClient;
	m_rcIcon.right = m_rcIcon.left + szIcn.cx;
	m_rcIcon.OffsetRect(0,(m_rcIcon.Height()  - szIcn.cy)/2);

	m_rcBtn = rcClient;
	m_rcBtn.left = m_rcBtn.right - 10;

	m_rcColor = rcClient;
	m_rcColor.left  = m_rcIcon.right + 2;
	m_rcColor.right = m_rcBtn.left   - 2;

	VERIFY(m_tooltip.Create(this));
	m_tooltip.AddTool(this);
	m_tooltip.Activate(TRUE);
	EnableToolTips(TRUE);

	return TRUE;
}

BOOL ColorBoxEx::OnNeedText(UINT id,NMHDR * pNMHDR,LRESULT * pResult) {
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	m_strTooltip.Format("#08X",GetColor());
	pTTT->lpszText = (LPTSTR)static_cast<LPCTSTR>(m_strTooltip);
	return TRUE;
}

BOOL ColorBoxEx::PreTranslateMessage(MSG*p) {
	m_tooltip.RelayEvent(p);
	return CWnd::PreTranslateMessage(p);
}

void ColorBoxEx::OnPaint()  {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBoxEx)));
	CPaintDC dc(this);

	BOOL bEnabled = IsWindowEnabled();
	BOOL bFocus   = CWnd::GetFocus()->GetSafeHwnd() == GetSafeHwnd();
	
	CRect rcClient;
	GetClientRect(rcClient);
	dc.FillSolidRect(rcClient,GetSysColor(COLOR_BTNFACE));
	if (bFocus) {
		dc.DrawFocusRect(rcClient);
	}
	
	if (bEnabled) {
		m_image.Draw(&dc,0,m_rcIcon.TopLeft(),ILD_NORMAL);
	} else {
		HICON hIcon=m_image.ExtractIcon(0);
		ASSERT(hIcon);
		dc.DrawState(m_rcIcon.TopLeft(),CSize(0,0),hIcon,DSS_DISABLED,(HBRUSH)NULL);
		DestroyIcon(hIcon);
	}

	if (bEnabled) {
		FillSolidRect(&dc,m_rcColor,m_cr);
	} else {
		dc.FillSolidRect(m_rcColor,GetSysColor(COLOR_GRAYTEXT));
	}

	CRect rc = m_rcColor;
	dc.Draw3dRect(rc,::GetSysColor(COLOR_3DDKSHADOW),::GetSysColor(COLOR_3DHILIGHT));
	rc.DeflateRect(1,1);
	dc.Draw3dRect(rc,::GetSysColor(COLOR_3DSHADOW),  ::GetSysColor(COLOR_3DLIGHT));

	CPoint ptArrowOff(0,0);
	if (m_bDown) {
		ptArrowOff = CPoint(1,1);
		CRect rc = m_rcBtn;
		dc.Draw3dRect(rc,::GetSysColor(COLOR_3DDKSHADOW),::GetSysColor(COLOR_3DHILIGHT));
		rc.DeflateRect(1,1);
		dc.Draw3dRect(rc,::GetSysColor(COLOR_3DSHADOW),  ::GetSysColor(COLOR_3DLIGHT));
	}

	CFont* pSaved = dc.SelectObject(&f_fontArrow);
	ASSERT(pSaved != NULL);
	int nSaved = dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(::GetSysColor(bEnabled ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
	dc.DrawText(_T("6"),m_rcBtn,DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	dc.SetBkMode(nSaved);
	VERIFY(dc.SelectObject(pSaved) != NULL);
}

void ColorBoxEx::OnLButtonDown(UINT,CPoint pt) {
	SetFocus();
	if (m_rcBtn.PtInRect(pt)) {
		PressButton();
	}
}

void ColorBoxEx::PressButton() {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBoxEx)));
	if (!m_bDown) {
		m_bDown = TRUE;
		Invalidate();
	}

	COLORREF cr = GetColor();
	if (GetParent()->SendMessage(WM_CLRBOXEX_SELECT,GetDlgCtrlID(),reinterpret_cast<LPARAM>(&cr))) {
		SetColor(cr);

		CWnd*   pwnd   = AfxGetMainWnd();
		WPARAM  wparam = MAKEWPARAM(GetDlgCtrlID(),0);
		LPARAM	lparam = reinterpret_cast<LPARAM>(this->GetSafeHwnd());
		
		ASSERT(pwnd != NULL && ::IsWindow(pwnd->GetSafeHwnd()));
		VERIFY(pwnd->PostMessage(WM_COMMAND,wparam,lparam));
	}

	m_bDown = FALSE;
	Invalidate();

}

COLORREF ColorBoxEx::GetCurrentColor() {
	HWND hwnd = reinterpret_cast<HWND>(AfxGetApp()->m_msgCur.lParam);
	ASSERT(::IsWindow(hwnd));

	ColorBoxEx* pThis = reinterpret_cast<ColorBoxEx*>(CWnd::FromHandle(hwnd));
	ASSERT_VALID(pThis);
	ASSERT(LOWORD(AfxGetApp()->m_msgCur.wParam) == pThis->GetDlgCtrlID());

	return pThis->GetColor();
}

UINT ColorBoxEx::OnGetDlgCode() {
	ASSERT_VALID(this);
	ASSERT(IsKindOf(RUNTIME_CLASS(ColorBoxEx)));

	return DLGC_BUTTON; 
}

void ColorBoxEx::OnKeyDown(UINT c,UINT r,UINT f) {
	if (c == VK_RETURN || c == VK_SPACE) {
		PressButton();
	} else {
		CWnd::OnKeyDown(c,r,f);
	}
}

void DDX_ReplaceWnd(CDataExchange* pDX, int nIDC,CWnd& wnd,DWORD style,DWORD styleEx) {
	if (wnd.m_hWnd != NULL) {
		return;
	}

	ASSERT(!pDX->m_bSaveAndValidate);
	ASSERT(WS_CHILD & style);
	
	CWnd* pLoc = CWnd::FromHandle(pDX->PrepareCtrl(nIDC));
	if (pLoc == NULL) {
		ASSERT(FALSE);
		return;
	}
	CWnd* pDlg = pDX->m_pDlgWnd;
	ASSERT_VALID(pDlg);

	CRect rc;
	pLoc->GetWindowRect(rc);
	pDlg->ScreenToClient(rc);

	if (!wnd.CreateEx(styleEx,NULL,NULL,style,rc,pDlg,nIDC)) {
		ASSERT(FALSE);
		AfxThrowNotSupportedException();
	}
	wnd.ShowWindow(SW_SHOW);
	wnd.UpdateWindow();
	wnd.SetWindowPos(pLoc,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
	if (pLoc->GetStyle() & WS_TABSTOP) {
		wnd.ModifyStyle(0,WS_TABSTOP);
	}
	VERIFY(pLoc->DestroyWindow());
}

void ColorBoxEx::DDX_Control(CDataExchange* pDX, int nIDC,ColorBoxEx& wnd,UINT nIconBmpId,COLORREF crMask) {
	wnd.m_idIcon = nIconBmpId;
	wnd.m_crMask = crMask;
	DDX_ReplaceWnd(pDX,nIDC,wnd,WS_CHILD,0);
}

void ColorBoxEx::DDX_COLORREF(CDataExchange* pDX,int nIDC,COLORREF& cr) {
	ColorBoxEx* colorbox = DYNAMIC_DOWNCAST(ColorBoxEx,pDX->m_pDlgWnd->GetDlgItem(nIDC));
	ASSERT_KINDOF(ColorBoxEx,colorbox);
	ASSERT_VALID(colorbox);
	if (pDX->m_bSaveAndValidate) {
		cr = colorbox->GetColor();
	} else {
		colorbox->SetColor(cr);
	}
}

BOOL ColorBoxEx::Create(CWnd* pParent,CRect rc,UINT nID,UINT nIconBmpId,COLORREF crMask) {
	m_idIcon = nIconBmpId;
	m_crMask = crMask;
	if (!CWnd::CreateEx(0,NULL,NULL,WS_CHILD | WS_TABSTOP,rc,pParent,nID)) {
		ASSERT(FALSE);
		return FALSE;
	}
	ShowWindow(SW_SHOW);
	UpdateWindow();
	return TRUE;
}

void ColorBoxEx::OnUpdateCmdUI(CCmdUI* pCmdUI,COLORREF crSet) {
	if (pCmdUI->m_pOther == NULL) {
		ASSERT(FALSE);
		return;
	}
	CWnd* pWnd = CWnd::FromHandlePermanent(pCmdUI->m_pOther->GetSafeHwnd());
	if (pWnd == NULL) {
		ASSERT(FALSE);
		return;
	}
	ColorBoxEx* pCB = DYNAMIC_DOWNCAST(ColorBoxEx,pWnd);
	ASSERT_VALID(pCB);
	pCB->SetColor(crSet);
}

ColorPicker::ColorPicker(CWnd* pParent,UINT curid)
:	m_pListener(NULL),
	m_hcurPicker(NULL),
	m_pParent(pParent) {
	ASSERT_VALID(pParent);
	m_hcurPicker = AfxGetApp()->LoadCursor(curid);
	if (m_hcurPicker == NULL) {
		AfxThrowResourceException();
	}
}

BOOL ColorPicker::DoPick() {
	CursorCapture cursorCapture;
	cursorCapture.SetCapture(m_pParent->GetSafeHwnd());
	
	HCURSOR hcurSaved = SetCursor(m_hcurPicker);

	HWND hwndPickup = ::GetDesktopWindow();
	HDC hdcPickup = ::GetDCEx(hwndPickup,NULL,DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
	::AfxLockTempMaps();

	BOOL bSpoited = FALSE;
	for(;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != m_pParent)
			break;

		CPoint pt = msg.pt;
		VERIFY(::ScreenToClient(hwndPickup,&pt));
		switch (msg.message) 
		{
		case WM_MOUSEMOVE:
			m_cr = ::GetPixel(hdcPickup,pt.x,pt.y);
			if (m_cr != CLR_INVALID  && m_pListener != NULL)
				m_pListener->OnPickerMove(pt,m_cr);
			break;

		case WM_LBUTTONUP:
			m_cr = ::GetPixel(hdcPickup,pt.x,pt.y);
			bSpoited = m_cr != CLR_INVALID;
			if (bSpoited && m_pListener != NULL)
				m_pListener->OnPickerMove(pt,m_cr);
			goto for_exit;

		case WM_KEYDOWN:
			goto for_exit;
		case WM_RBUTTONDOWN:					
			goto for_exit;

		default:
			DispatchMessage(&msg);
			break;
		}
	}
for_exit:
	::AfxUnlockTempMaps(FALSE);
	::ReleaseDC(hwndPickup,hdcPickup);
	SetCursor(hcurSaved);
	return bSpoited;
}

void ColorPicker::SetListener(ColorPickerListener* pListener) {
	m_pListener = pListener;
}

COLORREF ColorPicker::GetColor() const {
	return m_cr;
}
