#include "stdafx.h"
#include "InToolBar.h"
#include <afxpriv.h>
#include <algorithm>
#include "utility/PaintMemDC.h"
#include "utility/PxLib.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(InToolBar, CWnd)
	//{{AFX_MSG_MAP(InToolBar)
	ON_WM_GETDLGCODE()
		ON_WM_PAINT()
		ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0,OnNeedText)
	ON_MESSAGE_VOID(WM_MOUSELEAVE,OnMouseLeave)
	ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(InToolBar,CWnd);
InToolBar::InToolBar
(
	BOOL bBorder,
	CSize szBorder,
	BOOL bBmpBorder,
	CSize szBmpBorder
)
:	m_nHover(-1),
	m_nColCount(-1),
	m_bBorder(bBorder),
	m_szBorder(szBorder),
	m_bBmpBorder(bBmpBorder),
	m_szBmpBorder(szBmpBorder)
{
}

InToolBar::~InToolBar()
{
}

BOOL InToolBar::IsRealyEnabled() const {
	if (!IsWindowEnabled())
		return FALSE;
	CWnd* pParent = GetParent();
	while (pParent != NULL) {
		if (!pParent->IsWindowEnabled()) {
			return FALSE;
		}
		pParent = pParent->GetParent();
	}
	return TRUE;
}

UINT InToolBar::GetCurrentPosID() const {
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	int index = HitTest(point);
	if (-1 == index)
		return 0;
	return GetButtonID(index);
}

BOOL InToolBar::OnNeedText(UINT id,NMHDR * pNMHDR,LRESULT * pResult) {
	const UINT nStringID = GetCurrentPosID();
	if (nStringID == 0) {
		return FALSE;
	}

	TCHAR szFullText[256];
	AfxLoadString(nStringID,szFullText);
	AfxExtractSubString(m_strTooltip,szFullText,1,'\n');
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	pTTT->lpszText = (LPTSTR)static_cast<LPCTSTR>(m_strTooltip);

	return TRUE;
}

BOOL InToolBar::PreTranslateMessage(MSG* pMsg) {
	m_tooltip.RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL InToolBar::Create (
	UINT nResID,
	CWnd* pParentWnd,
	int nColCount,
	DWORD dwStyle,
	UINT nID
) {
	m_nColCount = nColCount;
	ASSERT(m_nColCount == -1 || 0 < m_nColCount);
	if (!LoadToolbar(nResID)) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_nColCount == -1) {
		m_szFrame.cx = m_szBorder.cx * 2 + m_szButton.cx * GetCount();
		m_szFrame.cy = m_szBorder.cy * 2 + m_szButton.cy;
	} else {
		int nrow = (GetCount() + m_nColCount - 1)/ m_nColCount;

		m_szFrame.cx = m_szBorder.cx * 2 + m_szButton.cx * m_nColCount;
		m_szFrame.cy = m_szBorder.cy * 2 + m_szButton.cy * nrow;
	}
	CRect rc;
	rc.SetRect(0,0,m_szFrame.cx,m_szFrame.cy);
	LPCTSTR ptszClassName = ::AfxRegisterWndClass(CS_OWNDC,::LoadCursor(NULL,IDC_ARROW),NULL,NULL);
	ASSERT(AfxIsValidString(ptszClassName));


	if (!CWnd::Create(ptszClassName,NULL,dwStyle,rc,pParentWnd,nID)) {
		return FALSE;
	}

	if (!Init()) {
		return FALSE;
	}
	return TRUE;
}

BOOL InToolBar::Init() {
	if (!m_tooltip.Create(this))
		return FALSE;

	int nButtons = GetCount();
	for (int n = 0;n < nButtons;n++) {
		CRect rc;
		GetButtonRect(n,rc);
		m_tooltip.AddTool(this,GetMessageString(GetButtonID(n)),rc,n + 1);
	}

	m_tooltip.Activate(TRUE);
	VERIFY(EnableToolTips(TRUE));
	return TRUE;
}

int InToolBar::GetCount() const
{
	int nCount = m_items.size();
	ASSERT(0 < nCount);
	return nCount;
}

void InToolBar::SetButtonStyle(int nIndex,UINT nStyle)
{
	InToolBarItem item;
	GetButtonItem(nIndex,item);
	BOOL bInvalidate = item.m_nState != nStyle;
	item.m_nState = nStyle;
	SetButtonItem(nIndex,item);
	if (bInvalidate)
		RedrawButton(nIndex);
}

UINT InToolBar::GetButtonStyle(int nIndex) const
{
	InToolBarItem item;
	GetButtonItem(nIndex,item);
	return item.m_nState;
}

UINT InToolBar::GetButtonID(int nIndex) const
{
	InToolBarItem item;
	GetButtonItem(nIndex,item);
	return item.m_nID;
}

void InToolBar::GetButtonRect(int nIndex,LPRECT prc) const
{
	InToolBarItem item;
	GetButtonItem(nIndex,item);
	
	*prc = item.m_rc;
}

void InToolBar::GetButtonItem(int nIndex,InToolBarItem& item) const
{
	item = m_items[nIndex];
}

void InToolBar::SetButtonItem(int nIndex,const InToolBarItem& item) {
	m_items[nIndex] = item;
}

class InToolBarCmdUI : public CCmdUI {
private:
	InToolBar* m_pToolBar;
public:
	InToolBarCmdUI(InToolBar*);
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetRadio(BOOL);
	virtual void SetText(LPCTSTR lpszText);
};

InToolBarCmdUI::InToolBarCmdUI(InToolBar* p)
:	m_pToolBar(p)
{
	m_pOther = p;
}

void InToolBarCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;

	ASSERT(m_pToolBar != NULL);
	ASSERT(m_nIndex < m_nIndexMax);

	int nStyle = m_pToolBar->GetButtonStyle(m_nIndex);
	BOOL bOld = !(nStyle & TBBS_DISABLED);
	UINT nNewStyle = nStyle & ~TBBS_DISABLED;
	if (!bOn)
	{
		nNewStyle |= TBBS_DISABLED;
		nNewStyle &= ~TBBS_PRESSED;
	}
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	m_pToolBar->SetButtonStyle(m_nIndex, nNewStyle);

	if ((!bOld && bOn) || (bOld && !bOn))
	{
		CRect rc;
		m_pToolBar->GetButtonRect(m_nIndex,rc);
		VERIFY(m_pToolBar->RedrawWindow(rc));
	}
}

void InToolBarCmdUI::SetCheck(int nCheck)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
	InToolBar* pToolBar = (InToolBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(InToolBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) &
				~(TBBS_CHECKED | TBBS_INDETERMINATE);
	if (nCheck == 1)
		nNewStyle |= TBBS_CHECKED;
	else if (nCheck == 2)
		nNewStyle |= TBBS_INDETERMINATE;
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX);
}

void InToolBarCmdUI::SetRadio(BOOL b)
{
	InToolBar* pToolBar = (InToolBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(InToolBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	if (b)
	{
		for (int n = 0;n < m_nIndexMax;n++)
		{
			UINT nOldStyle = pToolBar->GetButtonStyle(n);
			UINT nNewStyle = nOldStyle & ~(TBBS_CHECKED | TBBS_INDETERMINATE);
			if (n == m_nIndex)
				nNewStyle |= TBBS_CHECKED;
			pToolBar->SetButtonStyle(n,nNewStyle | TBBS_CHECKGROUP);
		}
	}
	else
	{
		UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) & ~(TBBS_CHECKED | TBBS_INDETERMINATE);
		pToolBar->SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKGROUP);
	}
}

void InToolBarCmdUI::SetText(LPCTSTR) {
}

void InToolBar::OnUpdateCmdUI(CWnd* pTarget,BOOL bDisableIfNoHndler) {
	if (!IsRealyEnabled()) {
		return;
	}

	InToolBarCmdUI state(this);
	state.m_pOther = this;

	state.m_nIndexMax = GetCount();
	for (ITEMS::iterator i = m_items.begin();
			             i != m_items.end();
						 i++)
	{
		InToolBarItem& item = *i;
		state.m_nIndex = item.m_nIndex;
		state.m_nID    = item.m_nID;

		if (!(item.m_nState & TBSTYLE_SEP)) {
			if (CWnd::OnCmdMsg(0,
				MAKELONG((int)CN_UPDATE_COMMAND_UI, WM_COMMAND+WM_REFLECT_BASE),
				&state, NULL))
				continue;

			if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
				continue;

			state.DoUpdate(pTarget,bDisableIfNoHndler);
		}
	}
}

void InToolBar::OnIdleUpdateCmdUI() {
	if (GetStyle() & WS_VISIBLE) {
		OnUpdateCmdUI(GetOwner(),TRUE);
	}
}

struct hit_by_point {
	CPoint m_pt;
	hit_by_point(CPoint pt) : m_pt(pt) {}
	BOOL operator () (const InToolBarItem& item) const
	{ return item.m_rc.PtInRect(m_pt) != FALSE; }
};

int InToolBar::HitTest(CPoint pt) const {
	if (pt.x < 0 || m_szFrame.cx <= pt.y) {
		return -1;
	}
	if (pt.y < 0 || m_szFrame.cy <= pt.y) {
		return -1;
	}

	ITEMS::const_iterator i = std::find_if(m_items.begin(),m_items.end(),hit_by_point(pt));
	if (i == m_items.end())
		return -1;
	else
	{
		const InToolBarItem& item = *i;
		return item.m_nIndex;
	}
}

void InToolBar::OnPaint() {
	PaintMemDC dc(this);

	DrawFrame(dc);

	ITEMS::iterator i;
	for (i = m_items.begin();i != m_items.end();i++) {
		InToolBarItem& item = *i;
		DrawButton(dc,item);
	}
}


void DrawBorder(CDC& dc,CRect rc,CSize szBorder)
{
	dc.Draw3dRect(rc,::GetSysColor(COLOR_BTNHILIGHT),::GetSysColor(COLOR_3DDKSHADOW));

	while (0 < szBorder.cx && 0 < szBorder.cy)
	{
		if (0 < szBorder.cx)
		{
			rc.DeflateRect(1,0);
			szBorder.cx--;
		}
		if (0 < szBorder.cy)
		{
			rc.DeflateRect(0,1);
			szBorder.cy--;
		}	
		dc.Draw3dRect(rc,::GetSysColor(COLOR_3DLIGHT),::GetSysColor(COLOR_BTNSHADOW));			
	}
}

void InToolBar::DrawFrame(CDC& dc)
{
	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc,::GetSysColor(COLOR_3DFACE));
	if (m_bBorder)
		DrawBorder(dc,rc,m_szBmpBorder);
}

void InToolBar::DrawButton(CDC& dc,const InToolBarItem& item) {
	CRect rc = item.m_rc;
	BOOL bEnabled = IsRealyEnabled() && !(item.m_nState & TBBS_DISABLED);
	BOOL bPressed = item.m_nState & TBBS_PRESSED || (item.m_nState & TBBS_CHECKED);

	if (bEnabled) {
		m_imagelistButtons.Draw(&dc,
								item.m_nIndex,
								rc.TopLeft() + (bPressed ? CPoint(1,1) : CPoint(0,0)),
								ILD_TRANSPARENT);

		if (bPressed)
			dc.DrawEdge(rc,BDR_SUNKENINNER,BF_RECT);
		else if (item.m_nIndex == m_nHover)
			dc.DrawEdge(rc,BDR_RAISEDINNER,BF_RECT);
		else if (m_bBmpBorder)
			DrawBorder(dc,rc,m_szBmpBorder);
	} else {
		PxLib::DrawEmbossed(dc,m_imagelistButtons,item.m_nIndex,rc.TopLeft());
	}
}

void InToolBar::RedrawButton(int nIndex) {
	ASSERT(0 <= nIndex && nIndex < GetCount());

	CRect rc;
	GetButtonRect(nIndex,rc);
	VERIFY(RedrawWindow(rc));
}

void InToolBar::OnMouseLeave() {
	int nOld = m_nHover;
	if (nOld != -1) {
		m_nHover = -1;
		RedrawButton(nOld);
	}
}

void InToolBar::OnMouseMove(UINT,CPoint point) {
	if (!IsRealyEnabled())
		return;

	int nOldHover = m_nHover;
	int nNewHover = HitTest(point);
	if (nNewHover != m_nHover)
	{
		m_nHover = nNewHover;
	
		if (nOldHover != -1)
			RedrawButton(nOldHover);
		if (nNewHover != -1)
			RedrawButton(nNewHover);
	}

	if (nOldHover == -1)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwHoverTime = 0;
		tme.hwndTrack   = m_hWnd;
		tme.dwFlags     = TME_LEAVE;
		VERIFY(_TrackMouseEvent(&tme));
	}
}


void InToolBar::OnLButtonDown(UINT,CPoint point) {
	if (!IsRealyEnabled())
		return;

	int nHit = HitTest(point);
	if (nHit == -1)
		return;
	
	UINT nStyle = GetButtonStyle(nHit);
	if (nStyle & TBBS_DISABLED)
		return;
	
	CRect rcItem;
	GetButtonRect(nHit,rcItem);

	SetButtonStyle(nHit,nStyle | TBBS_PRESSED);
	SetCapture();
	RedrawWindow(rcItem);
	
	::AfxLockTempMaps();
	MSG msg;
	for(;;) {
		VERIFY(::GetMessage(&msg,NULL,0,0));
		if (CWnd::GetCapture() != this)
			goto loop_exit;

		switch (msg.message) 
		{
		case WM_MOUSEMOVE:
			RedrawWindow(rcItem);
			break;
		case WM_LBUTTONUP:
			goto loop_exit;
		default:
			DispatchMessage(&msg);
		}
	}
loop_exit:
	VERIFY(ReleaseCapture());
	::AfxUnlockTempMaps(FALSE);
	nStyle = nStyle & (~TBBS_PRESSED);
	SetButtonStyle(nHit,nStyle);
	RedrawWindow(rcItem);

	CPoint pt;
	VERIFY(GetCursorPos(&pt));
	ScreenToClient(&pt);
	if (rcItem.PtInRect(pt)) {
		GetParent()->SendMessage(WM_COMMAND,GetButtonID(nHit));
	}
}


struct find_by_id {
	UINT m_nID;
	find_by_id(UINT nID) : m_nID(nID) {}
	BOOL operator () (const InToolBarItem& item) const { return m_nID == item.m_nID; }
};

int InToolBar::FindIndex(UINT nID) {
	ITEMS::const_iterator i = std::find_if(m_items.begin(),m_items.end(),find_by_id(nID));
	if (i == m_items.end())
		return -1;
	else
	{
		const InToolBarItem& item = *i;
		return item.m_nIndex;
	}
}

BOOL InToolBar::LoadToolbar(UINT nIDToolbar) {
	CBitmap bmpToolbar;

	HBITMAP hbmToolbar = PxLib::LoadSysColorBitmap(nIDToolbar);
	if (!hbmToolbar) 
	{
		ASSERT(FALSE);
		return FALSE;
	}
	bmpToolbar.Attach(hbmToolbar);

	// load toolbar
	struct TOOLBARDATA 
	{
		WORD wVersion;		// version # should be 1
		WORD wWidth;		// width of one bitmap
		WORD wHeight;		// height of one bitmap
		WORD wItemCount;	// number of items
		WORD items[1];		// array of command IDs, actual size is wItemCount
	};
	TOOLBARDATA* ptbd = NULL;
	{
		LPTSTR lpResName = MAKEINTRESOURCE(nIDToolbar);
		HINSTANCE    hInst;
		HRSRC        hRsrc;
		
		if ((hInst= AfxFindResourceHandle(lpResName, RT_TOOLBAR)) == NULL ||
			 (hRsrc= FindResource(hInst, lpResName, RT_TOOLBAR))   == NULL ||
			 (ptbd = (TOOLBARDATA*)LoadResource(hInst, hRsrc))     == NULL) 
		{
			TRACE(_T("*** Can't load toolbar %d!\n"), nIDToolbar);
			return FALSE;
		}
		ASSERT(ptbd->wVersion==1);
	}
	
	m_szBitmap = CSize(ptbd->wWidth, ptbd->wHeight);
	m_szButton = m_szBitmap + m_szBmpBorder + m_szBmpBorder;
	VERIFY(m_imagelistButtons.Create(m_szBitmap.cx,m_szBitmap.cy,ILC_MASK,0,10));

	m_imagelistButtons.Add(&bmpToolbar,GetSysColor(COLOR_3DFACE));
	
	for (int i = 0; i < ptbd->wItemCount; i++) 
	{
		UINT nID = ptbd->items[i];
		if (nID > 0) 
		{
			if (-1 == FindIndex(nID)) 
			{
				InToolBarItem item;
				item.m_nID = nID;
				item.m_nIndex = i;
				item.m_nState = 0;
				{
					int nLeft;
					int nTop;
					if (m_nColCount == -1)
					{
						nLeft = m_szBorder.cx + m_szButton.cx * i;
						nTop  = m_szBorder.cy;
					}
					else
					{
						ASSERT(0 < m_nColCount);
						int nrow = i / m_nColCount;
						int ncol = i % m_nColCount;

						nLeft = m_szBorder.cx + m_szButton.cx * ncol;
						nTop  = m_szBorder.cy + m_szButton.cy * nrow;
					}
					item.m_rc.SetRect(nLeft,nTop,nLeft + m_szButton.cx,nTop+m_szButton.cy);
				}
				m_items.push_back(item);
			}
			else
			{
				ASSERT(FALSE);
				TRACE(_T("*** Duplicate button ID %d ignored\n"), nID);
			} 
		}
	}
	
	bmpToolbar.Detach();

	return TRUE; // success!
}

UINT InToolBar::OnGetDlgCode() {
	return DLGC_BUTTON;	
}

void InToolBar::OnEnable(BOOL bEnable) {
	CWnd::OnEnable(bEnable);
}

BOOL InToolBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	if (nID   == 0 && 
	    nCode == MAKELONG(-1,WM_COMMAND+WM_REFLECT_BASE) &&
		pHandlerInfo == NULL) {
		return TRUE;
	}
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
