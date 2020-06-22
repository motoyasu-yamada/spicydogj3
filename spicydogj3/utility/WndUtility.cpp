#include "stdafx.h"
#include "WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CreateImageListColor32(CImageList& il,UINT nResID,int cx,int cy,COLORREF crMask) {
	CBitmap bmp;
	if (!bmp.LoadBitmap(nResID)) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (!il.Create(cx,cy,ILC_MASK | ILC_COLOR32,0,1)) {
		ASSERT(FALSE);
		return FALSE;
	}

	if (il.Add(&bmp,crMask) == -1) {
		ASSERT(FALSE);
		return FALSE;
	}

	bmp.Detach();
	return TRUE;
}

void LoadWindowPlacement(CWnd* pWnd,LPCTSTR ptszSec,LPCTSTR ptszEntry) {
	ASSERT_VALID(pWnd);
	ASSERT(AfxIsValidString(ptszSec));
	ASSERT(AfxIsValidString(ptszEntry));

	WINDOWPLACEMENT w;
	if (LoadWindowPlacementFromReg(ptszSec,ptszEntry,w)) {
		SetWindowPlacement(pWnd,w);
	} else {
		pWnd->ShowWindow(AfxGetApp()->m_nCmdShow);
	}
}

void SaveWindowPlacement(CWnd* pWnd,LPCTSTR ptszSec,LPCTSTR ptszEntry) {
	ASSERT_VALID(pWnd);
	ASSERT(AfxIsValidString(ptszSec));
	ASSERT(AfxIsValidString(ptszEntry));

	WINDOWPLACEMENT w;
	GetWindowPlacement(pWnd,w);
	SaveWindowPlacementToReg(ptszSec,ptszEntry,w);
}

const LPCTSTR WINDOWPLACEMENT_FORMAT = _T("%i,%i,(%i,%i),(%i,%i),(%i,%i)-(%i,%i)");
CString WindowPlacementToString(const WINDOWPLACEMENT& w) {
	ASSERT(AfxIsValidAddress(&w,sizeof w,FALSE));
	CString str;
	str.Format(WINDOWPLACEMENT_FORMAT,w.flags,w.showCmd,w.ptMinPosition,w.ptMaxPosition,w.rcNormalPosition);
	return str;
}

BOOL StringToWindowPlacement(LPCTSTR ptsz,WINDOWPLACEMENT& w) {
	ASSERT(AfxIsValidAddress(&w,sizeof w));
	if (ptsz == NULL)
		return FALSE;
	ASSERT(AfxIsValidString(ptsz));
	if (lstrlen(ptsz) == 0)
		return FALSE;
	ZeroMemory(&w,sizeof w);
	w.length  = sizeof w;
	int cRead = _stscanf(ptsz,
				 WINDOWPLACEMENT_FORMAT,
				 &w.flags,&w.showCmd,
				 &w.ptMinPosition.x,&w.ptMinPosition.y,
				 &w.ptMaxPosition.x,&w.ptMaxPosition.y,
				 &w.rcNormalPosition.left,&w.rcNormalPosition.top,
				 &w.rcNormalPosition.right,&w.rcNormalPosition.bottom);
	ASSERT(cRead == 10);
	return cRead == 10;
}

void SetWindowPlacement(CWnd* pWnd,const WINDOWPLACEMENT& w) {
	ASSERT_VALID(pWnd);
	ASSERT(AfxIsValidAddress(&w,sizeof w));
	ASSERT(w.length == sizeof w);
	VERIFY(pWnd->SetWindowPlacement(&w));
}

void GetWindowPlacement(CWnd* pWnd,WINDOWPLACEMENT& w) {
	ASSERT_VALID(pWnd);
	ASSERT(AfxIsValidAddress(&w,sizeof w));

	if (!pWnd->GetWindowPlacement(&w)) {
		ASSERT(FALSE);
		return;
	}
	if (pWnd->IsZoomed()) {
		w.flags |= WPF_RESTORETOMAXIMIZED;
	}
}

BOOL LoadWindowPlacementFromReg(LPCTSTR ptszSec,LPCTSTR ptszEntry,WINDOWPLACEMENT& w) {
	ASSERT(AfxIsValidString(ptszSec));
	ASSERT(AfxIsValidString(ptszEntry));
	return StringToWindowPlacement(AfxGetApp()->GetProfileString(ptszSec,ptszEntry),w);
}

void SaveWindowPlacementToReg  (LPCTSTR ptszSec,LPCTSTR ptszEntry,const WINDOWPLACEMENT& w) {
	ASSERT(AfxIsValidString(ptszSec));
	ASSERT(AfxIsValidString(ptszEntry));
	AfxGetApp()->WriteProfileString(ptszSec,ptszEntry,WindowPlacementToString(w));
}


CView* GetActiveView() {
	CWnd* pMainWnd = AfxGetMainWnd();
	ASSERT_VALID(pMainWnd);

	if (!pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		return NULL;

	return static_cast<CFrameWnd*>(pMainWnd)->GetActiveFrame()->GetActiveView();
}

BOOL GetKeyStillDown(int nKey) {
	return GetAsyncKeyState(nKey) & 0x8000;
}

void DrawTabRect(CDC& dc,const CRect& rc,BOOL bSel);
void DrawTabText(CDC& dc,const CRect& rc,LPCTSTR ptsz,int nIconSize,int nLeftMargin);
void DrawTab
(
	CDC& dc,
	const CRect& rc,
	BOOL bSel,
	LPCTSTR ptsz,
	HICON hi,
	int nIconWidth,
	int nIconHeight,
	int nLeftMargin
)
{
	DrawTabRect(dc,rc,bSel);

	CPoint ptDraw(rc.left + nLeftMargin,rc.top + (rc.Height()-nIconHeight -2)/2 + 2);
	::DrawIconEx(dc,ptDraw.x,ptDraw.y,hi,nIconWidth,nIconHeight,0,NULL,DI_NORMAL);

	DrawTabText(dc,rc,ptsz,nIconWidth,nLeftMargin);
}

void DrawTab
(
	CDC& dc,
	const CRect& rc,
	BOOL bSel,
	LPCTSTR ptsz,
	CImageList& il,
	int nIndex,
	int nLeftMargin
)
{
	DrawTabRect(dc,rc,bSel);

	IMAGEINFO ii;
	il.GetImageInfo(nIndex,&ii);
	int nIconSize = ii.rcImage.bottom - ii.rcImage.top;
	ASSERT(0 < nIconSize);

	CPoint ptDraw(rc.left + nLeftMargin,rc.top + (rc.Height()-nIconSize -2)/2 + 2);
	VERIFY(il.Draw(&dc,nIndex,ptDraw,ILD_TRANSPARENT));

	DrawTabText(dc,rc,ptsz,nIconSize,nLeftMargin);
}

void DrawTabText(CDC& dc,const CRect& rc,LPCTSTR ptsz,int nIconSize,int nLeftMargin)
{
	CRect rcText(rc);
	rcText.left += nLeftMargin + nIconSize + nLeftMargin;
	rcText.top += 2;
	int nSavedMode = dc.SetBkMode(TRANSPARENT);
	{
		dc.DrawText(ptsz,rcText,DT_SINGLELINE | DT_VCENTER| DT_LEFT | DT_END_ELLIPSIS);
	}
	dc.SetBkMode(nSavedMode);
}

void DrawTabRect(CDC& dc,const CRect& rc,BOOL bSel)
{
	COLORREF crFrame   = ::GetSysColor(COLOR_WINDOWFRAME);
	COLORREF crHilight = ::GetSysColor(COLOR_3DHILIGHT);
	COLORREF crShadow  = ::GetSysColor(bSel ? COLOR_3DSHADOW : COLOR_3DDKSHADOW);
	COLORREF crBkgnd   = ::GetSysColor(bSel ? COLOR_3DFACE : COLOR_3DSHADOW);

	CRect rcBkgnd(rc.left+2,rc.top+2,rc.right-2+1,rc.bottom);
	dc.FillSolidRect(rcBkgnd,crBkgnd);

	CPen penFrame;
	VERIFY(penFrame.CreatePen(PS_SOLID,1,crFrame));
	CPen penShadow;
	VERIFY(penShadow.CreatePen(PS_SOLID,1,crShadow));
	CPen penHilight;
	VERIFY(penHilight.CreatePen(PS_SOLID,1,crHilight));

	CPen* saved = dc.SelectObject(&penHilight);
	ASSERT_VALID(saved);
	dc.MoveTo(rc.left+1,rc.bottom-1);
	dc.LineTo(rc.left+1,rc.top+2);
	dc.MoveTo(rc.left+2, rc.top+1);
	dc.LineTo(rc.right-1,rc.top+1);

	dc.SelectObject(&penShadow);
	dc.MoveTo(rc.right-1, rc.top+2);
	dc.LineTo(rc.right-1,rc.bottom-1);

	dc.SelectObject(&penFrame);
	dc.MoveTo(rc.left,rc.bottom-1);
	dc.LineTo(rc.left,rc.top+2);
	dc.LineTo(rc.left+2,rc.top);
	dc.LineTo(rc.right-2,rc.top);
	dc.LineTo(rc.right-1,rc.top+1);
	dc.LineTo(rc.right,rc.top+2);
	dc.LineTo(rc.right,rc.bottom-1);
	if (!bSel) {
		dc.LineTo(rc.left,rc.bottom-1);
	}
	VERIFY(dc.SelectObject(saved) != NULL);
}

int GetListCtrlCurSel(const CListCtrl& list) {
	ASSERT(list.GetStyle() & LVS_SINGLESEL);
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return -1;
	}
	int n = list.GetNextSelectedItem(pos);
	return n;
}

int GetVirtualLMouseButton() {
	return (!GetSystemMetrics(SM_SWAPBUTTON)) ? VK_LBUTTON : VK_RBUTTON;
}

int GetVirtualRMouseButton() {
	return (!GetSystemMetrics(SM_SWAPBUTTON)) ? VK_RBUTTON : VK_LBUTTON;
}

/**
	ドラッグを開始するか？クリックするか？を判定する

	(戻値)	
	TRUE	ドラッグ開始
	FALSE	クリック
	(使用方法)
	void CMyWnd::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		if (IsToStartDrag(this,pt))
		{
			//	ドラッグ処理
		}
		else
			//	クリック処理
	}
*/
BOOL IsToStartDrag(CWnd* pWnd,CPoint ptDown,BOOL bLeftOrRight) {
	if (pWnd == NULL) {
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT_VALID(pWnd);

	const int nDragButton = bLeftOrRight ? GetVirtualLMouseButton() : GetVirtualRMouseButton();
	const int DRAGING_DISTANCE = 6;

	::AfxLockTempMaps();
	if (pWnd != CWnd::GetCapture())
		pWnd->SetCapture();
	ASSERT(CWnd::GetCapture() == pWnd);

	BOOL bDrag = FALSE;
	for (;;) {
		CPoint pt;
		MSG msg;
		VERIFY(GetMessage(&msg,NULL,0,0) != -1);
		if (CWnd::GetCapture() != pWnd) {
			goto loop_break;
		}
		UINT nMsg = msg.message;
		if (nMsg == WM_MOUSEMOVE) {
			if (!GetKeyStillDown(nDragButton)) {
				goto loop_break;
			}
			pt = msg.pt;
			pWnd->ScreenToClient(&pt);
			pt -= ptDown;
			if (DRAGING_DISTANCE < abs(pt.x) || DRAGING_DISTANCE < abs(pt.y)) {
				bDrag = TRUE;
				goto loop_break;
			}
		} else if (WM_MOUSEFIRST <= nMsg && nMsg <= WM_MOUSELAST) {
			goto loop_break;
		} else if (WM_KEYDOWN == nMsg) {
			if (msg.wParam == VK_ESCAPE)
				goto loop_break;
		} else {
			DispatchMessage(&msg);
		}
	}
loop_break:
	VERIFY(ReleaseCapture());
	::AfxUnlockTempMaps(FALSE);

	return bDrag;
}

struct AFX_HANDLEINFO {
	size_t nOffsetX;    // offset within RECT for X coordinate
	size_t nOffsetY;    // offset within RECT for Y coordinate
	int nCenterX;       // adjust X by Width()/2 * this number
	int nCenterY;       // adjust Y by Height()/2 * this number
	int nHandleX;       // adjust X by handle size * this number
	int nHandleY;       // adjust Y by handle size * this number
	int nInvertX;       // handle converts to this when X inverted
	int nInvertY;       // handle converts to this when Y inverted
};
const AFX_HANDLEINFO _afxHandleInfo[] = {
	// corner handles (top-left, top-right, bottom-right, bottom-left
	{ offsetof(RECT, left), offsetof(RECT, top),        0, 0,  0,  0, 1, 3 },
	{ offsetof(RECT, right), offsetof(RECT, top),       0, 0, -1,  0, 0, 2 },
	{ offsetof(RECT, right), offsetof(RECT, bottom),    0, 0, -1, -1, 3, 1 },
	{ offsetof(RECT, left), offsetof(RECT, bottom),     0, 0,  0, -1, 2, 0 },
	// side handles (top, right, bottom, left)
	{ offsetof(RECT, left), offsetof(RECT, top),        1, 0,  0,  0, 4, 6 },
	{ offsetof(RECT, right), offsetof(RECT, top),       0, 1, -1,  0, 7, 5 },
	{ offsetof(RECT, left), offsetof(RECT, bottom),     1, 0,  0, -1, 6, 4 },
	{ offsetof(RECT, left), offsetof(RECT, top),        0, 1,  0,  0, 5, 7 }
};

int NormalizeHit(const CRect& rc,int nHandle) {
	ASSERT(nHandle <= 8 && nHandle >= -1);
	if (nHandle == CRectTracker::hitMiddle || nHandle == CRectTracker::hitNothing)
		return nHandle;

	const AFX_HANDLEINFO* pHandleInfo = &_afxHandleInfo[nHandle];
	if (rc.Width() < 0) {
		nHandle = (CRectTracker::TrackerHit)pHandleInfo->nInvertX;
		pHandleInfo = &_afxHandleInfo[nHandle];
	}
	if (rc.Height() < 0) {
		nHandle = (CRectTracker::TrackerHit)pHandleInfo->nInvertY;
	}
	return nHandle;
}

void GetHandleRect(const CRect& rc,int nHandle,CRect& rcHandle,int nHandleSize,int nStyle) {
	ASSERT(nHandle < 8);

	CRect rectT = rc;
	rectT.NormalizeRect();
	if ((nStyle & (CRectTracker::solidLine | CRectTracker::dottedLine)) != 0)
		rectT.InflateRect(+1, +1);

	nHandle = NormalizeHit(rc,nHandle);

	int size = nHandleSize;
	if (nStyle & CRectTracker::resizeOutside)
		rectT.InflateRect(size-1, size-1);

	int nWidth = rectT.Width();
	int nHeight = rectT.Height();

	const AFX_HANDLEINFO* pHandleInfo = &_afxHandleInfo[nHandle];
	rcHandle.left = *(int*)((BYTE*)&rectT + pHandleInfo->nOffsetX);
	rcHandle.top = *(int*)((BYTE*)&rectT + pHandleInfo->nOffsetY);
	rcHandle.left += size * pHandleInfo->nHandleX;
	rcHandle.top += size * pHandleInfo->nHandleY;
	rcHandle.left += pHandleInfo->nCenterX * (nWidth - size) / 2;
	rcHandle.top += pHandleInfo->nCenterY * (nHeight - size) / 2;
	rcHandle.right = rcHandle.left + size;
	rcHandle.bottom = rcHandle.top + size;
}

CString GetMessageString(UINT nID) {
	CString rMessage;
	LPTSTR lpsz = rMessage.GetBuffer(255);
	if (AfxLoadString(nID, lpsz) != 0) {
		lpsz = _tcschr(lpsz, '\n');
		if (lpsz != NULL)
			*lpsz = '\0';
	} else {
		TRACE1("Warning: no message line prompt for ID 0x%04X.\n", nID);
	}

	rMessage.ReleaseBuffer();
	return rMessage;
}

/**
	親ウインドウ座標系の子ウインドウの位置
*/
void GetParentClientRect(const CWnd& wnd,CRect& rc)
{
	ASSERT(::IsWindow(wnd.GetSafeHwnd()));

	wnd.GetWindowRect(rc);

	CWnd* pParent = wnd.GetParent();
	if (pParent != NULL)
	{
		ASSERT_VALID(pParent);
		pParent->ScreenToClient(rc);
	}
}

/**
	ウインドウサイズや位置を取得するためのヘルパー関数

*/
int  GetWindowHeight(const CWnd& wnd)
{
	ASSERT(::IsWindow(wnd.GetSafeHwnd()));
	CRect rc;
	wnd.GetClientRect(rc);
	ASSERT(rc.Height() >= 0);
	return rc.Height();
}

int	 GetWindowWidth(const CWnd& wnd)
{
	ASSERT(::IsWindow(wnd.GetSafeHwnd()));
	CRect rc;
	wnd.GetClientRect(rc);
	ASSERT(rc.Width() >= 0);
	return rc.Width();
}

int GetChildWindowLeft(const CWnd& wnd)
{
	CRect rc;
	GetParentClientRect(wnd,rc);
	return rc.left;
}

int GetChildWindowTop(const CWnd& wnd)
{
	CRect rc;
	GetParentClientRect(wnd,rc);
	return rc.top;
}

int GetChildWindowRight(const CWnd& wnd)
{
	CRect rc;
	GetParentClientRect(wnd,rc);
	return rc.right;
}

int GetChildWindowBottom(const CWnd& wnd)
{
	CRect rc;
	GetParentClientRect(wnd,rc);
	return rc.bottom;
}

/**
	子ウインドウを指定した位置へ移動させる

	MoveChildWindowToTop
	MoveChildWindowToBottom
	MoveChildWindowToLeft
	MoveChildWindowToRight
*/
void MoveChildWindowToTop(CWnd& wnd,int nTop)
{
	CRect rc;
	GetParentClientRect(wnd,rc);
	
	wnd.SetWindowPos(NULL,rc.left,nTop,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void MoveChildWindowToBottom(CWnd& wnd,int nBottom)
{
	CRect rc;
	GetParentClientRect(wnd,rc);

	MoveRectTo(rc,rc.left,nBottom - rc.Height());
	wnd.SetWindowPos(NULL,rc.left,rc.top,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
void MoveChildWindowToRight(CWnd& wnd,int nRight)
{
	CRect rc;
	GetParentClientRect(wnd,rc);

	MoveRectTo(rc,nRight - rc.Width(),rc.top);
	wnd.SetWindowPos(NULL,rc.left,rc.top,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
void MoveChildWindowToLeft(CWnd& wnd,int nLeft)
{
	CRect rc;
	GetParentClientRect(wnd,rc);

	wnd.SetWindowPos(NULL,nLeft,rc.top,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
void MoveChildWindowToLeftNext(CWnd& wnd,const CWnd& wndLeft)
{
	ASSERT(*wnd.GetParent() == *wndLeft.GetParent());

	CRect rc;
	GetParentClientRect(wnd,rc);

	CRect rcLeft;
	GetParentClientRect(wndLeft,rcLeft);

	MoveRectTo(rc,rcLeft.right,rc.top);

	wnd.SetWindowPos(NULL,rc.left,rc.top,0,0,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

/**
	ウインドウサイズを変更する
*/
void StretchChildWindowWidth(CWnd& wnd,int nWidth)
{
	CRect rc;
	GetParentClientRect(wnd,rc);

	wnd.SetWindowPos(NULL,0,0,nWidth,rc.Height(),SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void StretchChildWindowHeight(CWnd& wnd,int nHeight)
{
	CRect rc;
	GetParentClientRect(wnd,rc);

	wnd.SetWindowPos(NULL,0,0,rc.Width(),nHeight,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void StretchChildWindowToBottom(CWnd& wnd,int nBottom)
{
	CRect rc;
	GetParentClientRect(wnd,rc);

	StretchChildWindowHeight(wnd,nBottom - rc.top);
}

void StretchChildWindowToRight(CWnd& wnd,int nRight)
{
	CRect rc;
	GetParentClientRect(wnd,rc);

	StretchChildWindowWidth(wnd,nRight - rc.left);
}

/**
	領域を移動

	同一サイズのまま矩形を指定位置へ移動する
	
	@param rc 移動させる矩形
	@param x  矩形の移動先
	@param y  矩形の移動先
*/
void MoveRectTo(CRect& rc,int x,int y)
{
	rc = CRect(CPoint(x,y),rc.Size());
}

void TrackDlgBtnPopupMenu(CDialog* pParent,UINT nItemID,UINT nMenuID,int nSubMenu)
{
	CWnd* pItem = pParent->GetDlgItem(nItemID);
	if (pItem == NULL)
		AfxThrowResourceException();

	CRect rc;
	pItem->GetWindowRect(rc);
	CPoint pt(rc.left,rc.bottom);
	TrackNewPopupMenu(pt,pParent,nMenuID,nSubMenu);
}
void TrackNewPopupMenu(POINT pt,CWnd* pParent,UINT nID,int nSubMenu)
{
	CMenu menu;
	if (!menu.LoadMenu(nID))
		AfxThrowResourceException();

	CMenu* pmenuSub = menu.GetSubMenu(nSubMenu);
	if (pmenuSub == NULL)
		AfxThrowResourceException();

	pmenuSub->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN,pt.x,pt.y,pParent);
}
void TrackNewPopupMenu(CWnd* pParent,UINT nID,int nSubMenu)
{
	CPoint pt;
	VERIFY(GetCursorPos(&pt));
	TrackNewPopupMenu(pt,pParent,nID,nSubMenu);
}

void EnableDescendantWindows(HWND hWnd,BOOL b) {
	ASSERT(::IsWindow(hWnd));
	for (HWND hWndChild = ::GetTopWindow(hWnd); 
			  hWndChild != NULL;
			  hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT))
	{
		ASSERT(::IsWindow(hWndChild));
		EnableDescendantWindows(CWnd::FromHandle(hWndChild),b);
		::EnableWindow(hWndChild,b);
	}
}
/**
	EnableDescendantWindows

	@param	hWnd	このウインドウおよび子ウインドウを有効化または無効化します
	@param	b		TRUE:有効化する,FALSE:無効化する
*/
void EnableDescendantWindows(CWnd* pWnd,BOOL b) {	
	ASSERT_VALID(pWnd);
	HWND hWnd = pWnd->m_hWnd;
	EnableDescendantWindows(hWnd,b);
}

/**
	EnableDlgItem
	ウインドウの子コントロールを有効化/無効化する
	
	@param	pParent	親ウインドウ
	@param	nID		有効化/無効化を制御する対象の子コントロールのID
	@param	b		TRUE:有効化する,FALSE:無効化する
*/
void EnableDlgItem(CWnd* pParent,UINT nID,BOOL b) {
	ASSERT(pParent != NULL);
	HWND hwnd = pParent->GetSafeHwnd();
	ASSERT(::IsWindow(hwnd));
	HWND hwndChild = ::GetDlgItem(hwnd,nID);
	ASSERT(::IsWindow(hwndChild));
	::EnableWindow(hwndChild,b);
}

/**
	ダイアログの子コントロールの、親ウインドウ座標系での矩形を求める

	@param pParent	[IN] 親ウインドウ
	@param nID		[IN] 子コントロールID
	@param rc		[OUT] 矩形
*/
void GetDlgItemRect(const CWnd* pParent,UINT nID,CRect& rc)
{
	ASSERT_VALID(pParent);
	CWnd* pChild = pParent != NULL ? pParent->GetDlgItem(nID) : NULL;
	ASSERT_VALID(pChild);
	if (pChild != NULL)
	{
		pChild->GetWindowRect(rc);
		if (pParent != NULL)
			pParent->ScreenToClient(rc);
		else
			rc.SetRectEmpty();
	}
	else
		rc.SetRectEmpty();

	ASSERT(!rc.IsRectEmpty());
}