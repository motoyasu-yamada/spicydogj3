#include "stdafx.h"
#include "resource.h"
#include "ToolsBoxBar.h"
#include "dlg/ColorPaletteDlg.h"
#include "utility/StringUtility.h"
#include "utility/WndUtility.h"
#include "wnd/ScrollViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MARGIN_CX = 4;
const int MARGIN_CY = 6;
const int MARGIN_TBR_CX = 4;
const int MARGIN_TBR_CY = 4;
const int CX_TBR_BTN = 24;
const int CY_TBR_BTN = 24;
const int CONTENT_CX = (MARGIN_TBR_CX + CX_TBR_BTN + MARGIN_TBR_CX) * 2;
const int FRAMEWIDTH = MARGIN_CX + CONTENT_CX + MARGIN_CX;
const int COLORBOX_WIDTH  = CONTENT_CX;
const int COLORBOX_HEIGHT = 24;
const int SCALEBOX_HEIGHT = 20;

BEGIN_MESSAGE_MAP(ToolsBoxBar, SECControlBar)
	//{{AFX_MSG_MAP(ToolsBoxBar)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLRBOXEX_SELECT,OnClrBoxSelect)
END_MESSAGE_MAP()

ToolsBoxBar::ToolsBoxBar()
{
}

ToolsBoxBar::~ToolsBoxBar()
{
}

BOOL ToolsBoxBar::CreateMe(CWnd* pParentWnd,UINT nID) {
	ASSERT_VALID(pParentWnd);
	ASSERT(nID != 0);

	DWORD dwStyle   = CBRS_SIZE_FIXED | WS_VISIBLE | CBRS_LEFT | CBRS_RIGHT ;
	DWORD dwExStyle = CBRS_EX_COOL|CBRS_EX_BORDERSPACE|CBRS_EX_FLATSTYLE;
	if (!SECControlBar::Create(pParentWnd,LoadString(IDS_TITLE_TOOLSBOX),dwStyle,dwExStyle,nID))
		return FALSE;
	
	return TRUE;
}

BOOL ToolsBoxBar::Create(CWnd* pParentWnd,UINT nID) {
	ASSERT_VALID(pParentWnd);
	ASSERT(nID != 0);
	if (!CreateMe(pParentWnd,nID))
		return FALSE;

	CRect rcInside;
	GetInsideRect(rcInside);

	int oy = rcInside.top;
	VERIFY(m_wndLogo.Create(IDC_POINTER,IDB_TBLOGO,this,ID_APP_ABOUT));
	m_wndLogo.SetWindowPos(NULL,MARGIN_CX,oy + MARGIN_CY,0,0,SWP_NOSIZE | SWP_NOZORDER);
	
	m_wndTools.Create(TBSTYLE_WRAPABLE,CPoint(MARGIN_CX,GetChildWindowBottom(m_wndLogo) + MARGIN_CY),this,IDT_DRAW);
	m_wndTools.LoadToolBar(IDT_DRAW,2,MARGIN_TBR_CX,MARGIN_TBR_CY);

	int nToolsBottom = GetChildWindowBottom(m_wndTools);
	CRect rc;
	rc.SetRect(MARGIN_CX,MARGIN_CY + nToolsBottom,MARGIN_CX + COLORBOX_WIDTH,MARGIN_CY + nToolsBottom + COLORBOX_HEIGHT);
	VERIFY(m_wndPaint.Create(this,rc,ID_COLOR_CHANGE_PAINT,IDB_PAINT,RGB(0x00,0x80,0x80)));
	m_wndPaint.SetColor(RGB(00,00,0xFF));
	m_wndPaint.ShowWindow(SW_HIDE);
	m_wndPaint.EnableWindow(FALSE);

	rc.OffsetRect(0,MARGIN_CY + COLORBOX_HEIGHT);
	VERIFY(m_wndLine.Create(this,rc,ID_COLOR_CHANGE_LINE,IDB_LINE,RGB(0x00,0x80,0x80)));
	m_wndLine.SetColor(RGB(0xFF,00,00));

	rc.OffsetRect(0,MARGIN_CY + COLORBOX_HEIGHT);
	rc.bottom = rc.top + SCALEBOX_HEIGHT;
	m_wndScale.Create(this,rc.TopLeft(),rc.Width(),ID_SCROLLVIEWEX_SCALE);

	m_sz.cx = FRAMEWIDTH;
	m_sz.cy = GetChildWindowBottom(m_wndScale) + MARGIN_CY;

	return TRUE;
}

CSize ToolsBoxBar::CalcDynamicLayout(int nLength,DWORD dwMode) {
	return m_sz;
}

void ToolsBoxBar::OnUpdateCmdUI(CFrameWnd* pTarget,BOOL) {
	ASSERT_VALID(pTarget);
	ASSERT(pTarget->IsKindOf(RUNTIME_CLASS(CFrameWnd)));
	UpdateDialogControls(pTarget,TRUE);
}

LRESULT ToolsBoxBar::OnClrBoxSelect(WPARAM wparam,LPARAM lparam) {
	COLORREF* pCr = reinterpret_cast<COLORREF*>(lparam);
	CRect rc;
	UINT nID = LOWORD(wparam);
	if (nID == ID_COLOR_CHANGE_LINE)
		m_wndLine.GetWindowRect(rc);
	else
		m_wndPaint.GetWindowRect(rc);

	ColorPaletteDlg dlg(*pCr,TRUE,rc.left,rc.bottom);
	if (dlg.DoModal() == IDOK) {
		*pCr = dlg.GetColor();
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL ToolsBoxBar::OnEraseBkgnd(CDC* pDC) {
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc,::GetSysColor(COLOR_BTNFACE));
	return TRUE;
}

BOOL ToolsBoxBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	if (SECControlBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	if (AfxGetMainWnd()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	return FALSE;
}
