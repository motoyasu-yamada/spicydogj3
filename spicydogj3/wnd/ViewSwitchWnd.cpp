#include "stdafx.h"
#include "resource.h"
#include "ViewSwitchWnd.H"
#include "ViewSwitch.H"
#include "utility/PaintMemDC.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT ID_CLOSE_BTN = 1000;
const int POINTSIZE_TAB = 90;
const COLORREF MASKCOLOR_ICON = RGB(0x00,0x80,0x80);
const int VIEWSWITCHWND_HEIGHT = 28;
const int CLOSE_BTN_SIZE = 14;
const int ICONSIZE_TAB = 16;
const int WND_MARGIN_LEFT  = 8;
const int WND_MARGIN_RIGHT = 4;
const int WND_MARGIN_TOP = 4;
const int WND_MARGIN_BOTTOM = 3;
const int BTN_MARGIN_H = 3;


BEGIN_MESSAGE_MAP(ViewSwitchWnd, CWnd)
	//{{AFX_MSG_MAP(ViewSwitchWnd)
	ON_COMMAND(ID_CLOSE_BTN,OnCloseBtn)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


ViewSwitchWnd::ViewSwitchWnd(ViewSwitch& s)
:	m_switch(s),
	m_nActiveView(0),
	m_pActiveView(NULL)
{
	if (0 < m_switch.GetViewCount())
		m_nActiveView = m_switch.GetActiveView();
}


BOOL ViewSwitchWnd::Create(CWnd* parent) {
    ASSERT_VALID(parent);

	if (!CreateImageListColor32(m_ImageList,IDB_ICON,16,16,MASKCOLOR_ICON)) {
		ASSERT(FALSE);
		return FALSE;
	}

    CString wndclass = ::AfxRegisterWndClass(CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW),
        ::GetSysColorBrush(COLOR_BTNFACE), 0);
	if (!CWnd::Create(wndclass,NULL,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),parent,0)) {
		ASSERT(FALSE);
		return FALSE;
	}

	//	表示フォントを生成
	LOGFONT logfont;
	VERIFY(SystemParametersInfo(SPI_GETICONTITLELOGFONT,sizeof logfont,&logfont,0));
	VERIFY(m_fontTab.CreateFontIndirect(&logfont));
	
	SetFont(&m_fontTab,FALSE);

	CreateCloseButton();

	RecalcLayout();

	return TRUE;
}

//	ビュースイッチに登録されているビュー情報が変更された	
void ViewSwitchWnd::Update()
{
	//	アクティブビューが変更されていないなら
	int nNewActive = m_switch.GetActiveView();

	//	親フレームの取得
	CFrameWnd* pFrame = GetParentFrame();
	//	以前のアクティブビューを表示オフにする
	if (m_pActiveView == NULL)
	{
		m_pActiveView = pFrame->GetActiveView();
	}
	
	if (m_pActiveView != NULL && ::IsWindow(m_pActiveView->GetSafeHwnd()))
	{
		m_pActiveView->SetDlgCtrlID(AFX_IDW_PANE_FIRST + 1);
		m_pActiveView->ShowWindow(SW_HIDE);	
	}

	m_nActiveView = nNewActive;

	//	アクティブビューを切替
	ASSERT(0 <= nNewActive);
	ASSERT(nNewActive < m_switch.GetViewCount());
	ViewSwitchInfo infoNew;
	m_switch.GetViewInfo(nNewActive,infoNew);
	CView* pViewNew = infoNew.GetView();
	ASSERT_VALID(pViewNew);		
	pViewNew->ShowWindow(SW_SHOW);
	pViewNew->UpdateWindow();
	pViewNew->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	pFrame->SetActiveView(pViewNew);

	m_pActiveView = pViewNew;

	RecalcChildsLayout();
	pFrame->RecalcLayout();
}

void ViewSwitchWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	RecalcChildsLayout();
}


//	親ウインドウに合わせてレイアウトを変更する
void ViewSwitchWnd::RecalcLayout() {
	CWnd* parent = GetParent();
	ASSERT_VALID(parent);

	CRect rcParent;
	parent->GetClientRect(rcParent);
	SetWindowPos(NULL,0,0,rcParent.Width(),VIEWSWITCHWND_HEIGHT,SWP_NOACTIVATE | SWP_NOZORDER);

	//	子のレイアウト変更
	RecalcChildsLayout();
}

//	レイアウトを再計算する
void ViewSwitchWnd::RecalcChildsLayout()
{
	RecalcViewTabLayout();
	RecalcCloseBtn();
	Invalidate();
}

ViewSwitchWnd::~ViewSwitchWnd() {
}

BOOL ViewSwitchWnd::OnEraseBkgnd(CDC*) {
	return TRUE;
}

void ViewSwitchWnd::OnPaint()  {
	PaintMemDC dcMem(this);

	DrawBkgnd(dcMem);
	DrawViewTabs(dcMem);
	m_btnClose.OnPaint(&dcMem);
}

void ViewSwitchWnd::DrawBkgnd(CDC& dc)
{
	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc,GetSysColor(COLOR_BTNFACE));

	CBrush br;
	VERIFY(br.CreateSolidBrush(RGB(0x00,0x00,0x00)));
	rc.bottom -= WND_MARGIN_BOTTOM;
	dc.FrameRect(rc,&br);
}

void ViewSwitchWnd::DrawViewTabs(CDC& dc) {
	if (m_ViewTabs.size() ==0) {
		return;
	}

	CFont* saved = dc.SelectObject(&m_fontTab);
	ASSERT_VALID(saved);

	int nViews = m_switch.GetViewCount();
	int nActive = m_switch.GetActiveView();
	for (int n = 0;n < nViews;n++) {
		ViewSwitchInfo info;
		m_switch.GetViewInfo(n,info);

		const ViewTabInfo& tab = m_ViewTabs[n];
		DrawTab(dc,tab.m_rc,nActive == n,info.GetCaption(),m_ImageList,info.GetIcon(),BTN_MARGIN_H);
	}

	VERIFY(dc.SelectObject(saved) == &m_fontTab);
}

//	閉じるボタンのレイアウトを再計算する
void ViewSwitchWnd::RecalcCloseBtn()
{
	CRect rcWnd;
	GetClientRect(rcWnd);
	CRect rcBtn;
	rcBtn.right = rcWnd.right - WND_MARGIN_RIGHT;
	rcBtn.left = rcBtn.right - CLOSE_BTN_SIZE;
	rcBtn.top = (rcWnd.Height() - CLOSE_BTN_SIZE)/2;
	rcBtn.bottom = rcBtn.top + CLOSE_BTN_SIZE;
	m_btnClose.Move(rcBtn);

	int nTabCount = m_ViewTabs.size();
	if (nTabCount != 0)
	{
		int nActive = m_switch.GetActiveView();
		ASSERT(0 <= nActive);
		ASSERT(nActive < nTabCount);
		ViewSwitchInfo info;
		m_switch.GetViewInfo(nActive,info);
		if (info.IsClosable())
		{
			if (!m_btnClose.IsVisible())
				m_btnClose.Show(TRUE);
		}
		else
		{
			m_btnClose.Show(FALSE);
		}
	}
	else
	{
		m_btnClose.Show(FALSE);
	}
}

//	ビュー切替タブレイアウトを再計算する
void ViewSwitchWnd::RecalcViewTabLayout()
{
	//	総ビュー数
	int nViews = m_switch.GetViewCount();
	if (nViews == 0)
		return;
	//	初期化
	m_ViewTabs.clear();
	m_ViewTabs.resize(nViews);
	//	アクティブビューとそのサイズ
	int nActiveIndex = m_switch.GetActiveView();
	int nActiveSize  = 0;

	CClientDC dc(this);
	CFont* pSaved = dc.SelectObject(&m_fontTab);
	ASSERT_VALID(pSaved);

	//	タブの幅
	int nWndWidth;
	int nWndHeight;
	{
		CRect rc;
		GetClientRect(rc);
		nWndWidth = rc.Width() - CLOSE_BTN_SIZE - WND_MARGIN_RIGHT - WND_MARGIN_RIGHT;
		nWndHeight = rc.Height();
	}

	//	各タブの幅を単純に足し合わせた数値
	int nTotalWidth = 0;
	int nLeft = WND_MARGIN_LEFT;
	{
		for (int n = 0;n < nViews;n++)
		{
			ViewTabInfo& tab = m_ViewTabs[n];

			ViewSwitchInfo info;
			m_switch.GetViewInfo(n,info);
			int nw = BTN_MARGIN_H + ICONSIZE_TAB + 
					 BTN_MARGIN_H + dc.GetTextExtent(info.GetCaption()).cx + BTN_MARGIN_H;

			tab.m_rc.SetRect(nLeft,WND_MARGIN_TOP,nLeft+nw,nWndHeight-WND_MARGIN_BOTTOM);
			//TRACE("%d (%d,%d)-(%d,%d)\n",n,tab.m_rc);

			nTotalWidth += nw;
			nLeft += nw;

			if (n == nActiveIndex)
				nActiveSize = nw;
		}
	}

	if (1 < nViews && nWndWidth < nTotalWidth)
	{
		int nResize = (nWndWidth-nActiveSize) * 100 / (nTotalWidth-nActiveSize);
		int nLeft   = WND_MARGIN_LEFT;
		for (int n = 0;n < nViews;n++)
		{
			ViewTabInfo& tab = m_ViewTabs[n];
			
			int nTabWidth;
			if (n == nActiveIndex)
				nTabWidth = nActiveSize;
			else
				nTabWidth = (tab.m_rc.Width() * nResize) / 100;
			
			tab.m_rc.SetRect(nLeft,WND_MARGIN_TOP,nLeft+nTabWidth,nWndHeight-WND_MARGIN_BOTTOM);;
			//TRACE("%d (%d,%d)-(%d,%d)\n",n,tab.m_rc);
			nLeft += nTabWidth;
		}
	}

	VERIFY(dc.SelectObject(pSaved) != NULL);
}

//	閉じるボタンを生成する
void ViewSwitchWnd::CreateCloseButton()
{
	m_btnClose.Create(this);
}

//	ビューを閉じる
void ViewSwitchWnd::OnCloseBtn()
{
	ViewSwitchInfo info;
	int nActive = m_switch.GetActiveView();
	if (0 < nActive)
		m_switch.ActivateView(nActive);

	m_switch.GetViewInfo(nActive,info);
	if (info.IsClosable())
	{
		m_switch.RemoveView(nActive);
		CView* pView = info.GetView();
		if (pView != NULL)
			pView->DestroyWindow();
		else
			ASSERT(FALSE);
	}
	else
		ASSERT(FALSE);

	Update();
}


//	タブのヒットテスト
int ViewSwitchWnd::HitTabs(CPoint pt)
{
	int nViews = m_switch.GetViewCount();

	int x = pt.x;
	for (int n = 0;n < nViews;n++)
	{
		const ViewTabInfo& tab = m_ViewTabs[n];
		if (tab.m_rc.PtInRect(pt))
			return n;
	}
	return -1;
}


//	タブクリックでタブを切り替える
void ViewSwitchWnd::OnLButtonDown(UINT, CPoint point) 
{
    if (m_btnClose.HitTest(point))
		m_btnClose.OnLButtonDown(point);
	else
	{
		int nActive = m_switch.GetActiveView();
		int nHit = HitTabs(point);
		if (nHit != -1 && nHit != nActive)
		{
			// TRACE("OnLButtonUp (%d,%d) %d\n",point,nActive);
			m_switch.ActivateView(nHit);
			Update();
		}
	}
}

void ViewSwitchWnd::OnRButtonDown(UINT nFlags, CPoint point) {
	int nHit = HitTabs(point);
	if (nHit == -1) {
		TrackNewPopupMenu(GetParentFrame(),IDM_VIEWSWITCH,1);
	} else {
		int nActive = m_switch.GetActiveView();
		if (nHit != nActive) {
			m_switch.ActivateView(nHit);
			Update();
		}
		TrackNewPopupMenu(GetParentFrame(),IDM_VIEWSWITCH,0);
	}
}

ViewSwitchWnd::VirtualButton::VirtualButton()
:	m_bPushed(FALSE),
	m_pParent(NULL)
{
}

BOOL ViewSwitchWnd::VirtualButton::Create(CWnd* pwnd)
{
	m_pParent = pwnd;
	ASSERT_VALID(m_pParent);

	return TRUE;
}

//	ボタンの移動
void ViewSwitchWnd::VirtualButton::Move(CPoint pt)
{
	m_rcOrg = CRect(pt,m_rcOrg.Size());
}

//	ボタンの移動とリサイズ
void ViewSwitchWnd::VirtualButton::Move(const CRect& rc)
{
	m_rcOrg = rc;
}
//	ボタンの矩形を取得
void ViewSwitchWnd::VirtualButton::GetRect(CRect& rc)
{
	rc = m_rcOrg;
}

//	ヒットテスト
BOOL ViewSwitchWnd::VirtualButton::HitTest(CPoint pt)
{
    if (m_bVisible && m_rcOrg.PtInRect(pt))
        return TRUE;
	else
		return FALSE;
}

//	再描画
void ViewSwitchWnd::VirtualButton::Invalidate()
{
	CClientDC dc(m_pParent);
	OnPaint(&dc);
	//TRACE("ViewSwitchWnd::VirtualButton::Invalidate()\n");
}

//	表示/非表示
void ViewSwitchWnd::VirtualButton::Show(BOOL b)
{
	m_bVisible = b;
    Invalidate();
}

BOOL ViewSwitchWnd::VirtualButton::IsVisible() const
{
	return m_bVisible;
}

void ViewSwitchWnd::VirtualButton::OnPaint(CDC* pDC)
{
	if (!IsVisible())
		return;

    CRect rc;
	GetRect(rc);

	UINT ns = DFCS_CAPTIONCLOSE;
    if (m_bPushed)
        ns |= DFCS_PUSHED;

	VERIFY(pDC->DrawFrameControl(rc,DFC_CAPTION,ns));
}

void ViewSwitchWnd::VirtualButton::OnLButtonDown(CPoint pt)
{
	if ((!IsVisible()) || !HitTest(pt))
		return;
	
	m_bPushed = TRUE;
	m_pParent->SetCapture();
	Invalidate();

	::AfxLockTempMaps();
	MSG msg;
	for(;;) {
		VERIFY(::GetMessage(&msg,NULL,0,0));

		if (CWnd::GetCapture() != m_pParent)
			goto loop_exit;

		switch (msg.message) {
		case WM_MOUSEMOVE: {
			CPoint pt;
			VERIFY(GetCursorPos(&pt));
			m_pParent->ScreenToClient(&pt);
			m_bPushed = HitTest(pt);
			Invalidate();
		}
		break;
		case WM_LBUTTONUP:
			goto loop_exit;
		default:
			DispatchMessage(&msg);
		}
	}

loop_exit:
	::AfxUnlockTempMaps(FALSE);
	VERIFY(ReleaseCapture());
	m_bPushed = FALSE;
	Invalidate();

	CPoint ptLast;
	VERIFY(GetCursorPos(&ptLast));
	m_pParent->ScreenToClient(&ptLast);
	if (HitTest(ptLast)) {
		m_pParent->SendMessage(WM_COMMAND,ID_CLOSE_BTN);
	}
}
