#include "stdafx.h"
#include "resource.h"
#include "ScrollViewEx.h"

#include <AfxPriv.h>
#include "model/type/type.h"
#include "utility/StringUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double MAX_SCALE  =  10;
const double MIN_SCALE  =  0.1;
const int MM_NONE       =  0;
const int MM_SCALE_DOC2FIT   = -1;
const int MM_SCALE_TOTAL2FIT = -2;
const int MM_SCALE           = -3;


IMPLEMENT_DYNAMIC(ScrollViewEx, CView)
BEGIN_MESSAGE_MAP(ScrollViewEx, CView)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SCROLLVIEWEX_SCALE,OnScale)
	ON_UPDATE_COMMAND_UI(ID_SCROLLVIEWEX_SCALE,OnUpdateScale)
	ON_UPDATE_COMMAND_UI(ID_SCROLLVIEWEX_MOUSE,OnUpdateMouse)
END_MESSAGE_MAP()

/**
	四捨五入による倍精度浮動小数点から整数への変換
	C++の浮動小数点から整数のキャストは切捨です
*/

inline CSize operator * (const CSize& sz,double d) {
	return CSize(double_2_int(sz.cx * d),double_2_int(sz.cy * d));
}

inline CSize operator / (const CSize& sz,double d) {
	return CSize(double_2_int(sz.cx / d),double_2_int(sz.cy / d));
}

struct SAUX_DATA {
	int cxVScroll;
	int cyHScroll;
	UINT    nWinVer;        // Major.Minor version numbers
	BOOL    bWin95;         // TRUE if Windows 95 (not NT)
	BOOL    bWin4;          // TRUE if Windows 4.0
	BOOL    bMarked4;       // TRUE if marked as 4.0
	SAUX_DATA();
};
SAUX_DATA safxData;
SAUX_DATA::SAUX_DATA() {
	DWORD dwVersion = ::GetVersion();
	nWinVer = (LOBYTE(dwVersion) << 8) + HIBYTE(dwVersion);
	bWin95 = (dwVersion & 0x80000000) != 0;
	bWin4 = (BYTE)dwVersion >= 4;
	bMarked4 = FALSE;

	if (bWin4) {
		bMarked4 = (GetProcessVersion(0) >= 0x00040000);
	}

	if (bMarked4) {
		if (safxData.bWin4) {
			safxData.cxVScroll = GetSystemMetrics(SM_CXVSCROLL) + CX_BORDER;
			safxData.cyHScroll = GetSystemMetrics(SM_CYHSCROLL) + CY_BORDER;
			safxData.bMarked4 = TRUE;
		}
	} else {
		safxData.cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
		safxData.cyHScroll = GetSystemMetrics(SM_CYHSCROLL);
		safxData.bMarked4 = FALSE;
	}
}


UINT PASCAL _AfxGetMouseScrollLines() {
	static UINT uCachedScrollLines = 0;
	static BOOL _afxGotScrollLines = FALSE;

	if (_afxGotScrollLines)
		return uCachedScrollLines;

	_afxGotScrollLines = TRUE;

	static UINT msgGetScrollLines;
	static WORD nRegisteredMessage;

	if (nRegisteredMessage == 0)
	{
		msgGetScrollLines = ::RegisterWindowMessage(MSH_SCROLL_LINES);
		if (msgGetScrollLines == 0)
			nRegisteredMessage = 1;     // couldn't register!  never try again
		else
			nRegisteredMessage = 2;     // it worked: use it
	}

	if (nRegisteredMessage == 2)
	{
		HWND hwMouseWheel = NULL;
		hwMouseWheel = FindWindow(MSH_WHEELMODULE_CLASS, MSH_WHEELMODULE_TITLE);
		if (hwMouseWheel && msgGetScrollLines)
		{
			uCachedScrollLines = (UINT)
				::SendMessage(hwMouseWheel, msgGetScrollLines, 0, 0);
			return uCachedScrollLines;
		}
	}

	uCachedScrollLines = 3; // reasonable default
	if (!safxData.bWin4) //***
	{
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
				0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
		{
			TCHAR szData[128];
			DWORD dwKeyDataType;
			DWORD dwDataBufSize = sizeof szData / sizeof szData[0];

			if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
					(LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
			{
				uCachedScrollLines = _tcstoul(szData, NULL, 10);
			}
			RegCloseKey(hKey);
		}
	}
	else if (!safxData.bWin95) {
		::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uCachedScrollLines, 0);
	}

	return uCachedScrollLines;
}

ScrollViewEx::ScrollViewEx() : m_bInitialUpdated(FALSE),m_bUpdateScrollSizes(FALSE) {
	AFX_ZERO_INIT_OBJECT(CView);
	m_nMapMode = MM_NONE;
}

ScrollViewEx::~ScrollViewEx() {
}

BOOL ScrollViewEx::PreTranslateMessage(LPMSG lpmsg) {
    switch (lpmsg->message) {
    case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		SetFocus();
		break;
    }
    return FALSE;
}

void ScrollViewEx::OnUpdateMouse(CCmdUI* pCmd) {
	ASSERT(pCmd != NULL);
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	DPtoLP(&pt);
	pCmd->SetText(FormatString(pCmd->m_nID,pt));
}

void ScrollViewEx::SetScrollSizes(SIZE sizeDocument,int nTotalByDocument,POINT ptCenter) {
	ASSERT(m_hWnd != NULL && ::IsWindow(m_hWnd));
	m_documentLog      = sizeDocument;
	m_nTotalByDocument = nTotalByDocument;
	m_documentCenter   = ptCenter;
	m_totalLog.cx = m_documentLog.cx * nTotalByDocument;
	m_totalLog.cy = m_documentLog.cy * nTotalByDocument;

	if (m_nMapMode != MM_NONE) {
		UpdateScrollSizes();
	}
}

void ScrollViewEx::SetScale(double dScale) {
	m_nMapMode = MM_SCALE;
	dScale = std::_MAX(MIN_SCALE,dScale);
	dScale = std::_MIN(MAX_SCALE,dScale);
	m_scale   = dScale;
	UpdateScrollSizes();
}

void ScrollViewEx::Center(BOOL b) {
	m_bCenter = b;
}

/*
	ウィンドウからビューポートへの変換式 
	xViewport = (xWindow - xWinOrg) × (xViewExt ÷ xWinExt) + xViewOrg 
	yViewport = (yWindow - yWinOrg) × (yViewExt ÷ yWinExt) + yViewOrg 
	ビューポートからウィンドウへの変換式  
	xWindow = (xViewport - xViewOrg) × (xWinExt ÷ xViewExt) + xWinOrg 
	yWindow = (yViewport - yViewOrg) × (yWinExt ÷ yViewExt) + yWinOrg
*/

#define PREAPAREDC  ScrollViewEx* pTHIS = const_cast<ScrollViewEx*>(this);CClientDC dc(pTHIS);pTHIS->OnPrepareDC(&dc)
void ScrollViewEx::DPtoLP(LPPOINT ppt,int n) const {
	PREAPAREDC;
	DPtoLP(&dc,ppt,n);
}
void ScrollViewEx::LPtoDP(LPPOINT ppt,int n) const  {
	PREAPAREDC;
	LPtoDP(&dc,ppt,n);
}
void ScrollViewEx::DPtoLP(LPSIZE psz) const  {
	PREAPAREDC;
	DPtoLP(&dc,psz);
}
void ScrollViewEx::LPtoDP(LPSIZE psz) const  {
	PREAPAREDC;
	LPtoDP(&dc,psz);
}
void ScrollViewEx::DPtoLP(LPRECT prc) const  {
	PREAPAREDC;
	DPtoLP(&dc,prc);
}
void ScrollViewEx::LPtoDP(LPRECT prc) const  {
	PREAPAREDC;
	LPtoDP(&dc,prc);
}
HRGN ScrollViewEx::DPtoLP(HRGN hrgn) const  {
	PREAPAREDC;
	return DPtoLP(&dc,hrgn);
}
HRGN ScrollViewEx::LPtoDP(HRGN hrgn) const  {
	PREAPAREDC;
	return LPtoDP(&dc,hrgn);
}

void ScrollViewEx::LPtoDP(CBEZIER& cb) {
	PREAPAREDC;
	LPtoDP(&dc,cb);
}

void ScrollViewEx::LPtoDP(CDC* pDC,CBEZIER& cb) {
	const int num = cb.GetSize();
	for (int n = 0;n < num;n++) {
		BEZIER& b = cb[n];
		LPtoDP(pDC,b.GetData(),b.GetSize());
	}
}

HRGN ScrollViewEx::LPtoDP(CDC* pDC,HRGN hrgn) {
	DWORD dwSize = ::GetRegionData(hrgn,0,NULL);
	CArray<BYTE,BYTE&> buffer;
	buffer.SetSize(sizeof(::RGNDATA) + dwSize);
	RGNDATA* pData = (RGNDATA*)buffer.GetData();
	if (::GetRegionData(hrgn,dwSize,pData) == 0) {
		ASSERT(FALSE);
		return NULL;
	}
	CPoint ptWOrg = pDC->GetWindowOrg();
	CSize  szWExt = pDC->GetWindowExt();
	CPoint ptVOrg = pDC->GetViewportOrg();
	CSize  szVExt = pDC->GetViewportExt();
	double xVW = static_cast<double>(szVExt.cx) / szWExt.cx;
	double yVW = static_cast<double>(szVExt.cy) / szWExt.cy;
	XFORM xform;
	xform.eM11 = xVW;
	xform.eM12 = 0;
	xform.eDx  = ptVOrg.x - ptWOrg.x * xVW;
	xform.eM21 = 0;
	xform.eM22 = yVW;
	xform.eDy  = ptVOrg.y - ptWOrg.y * yVW;
	HRGN hrgnDst = ::ExtCreateRegion(&xform,dwSize,pData);
	ASSERT(hrgnDst != NULL);
	return hrgnDst;
}

HRGN ScrollViewEx::DPtoLP(CDC* pDC,HRGN hrgn) {
	RECT rc;
	::GetRgnBox(hrgn,&rc);

	DWORD dwSize = ::GetRegionData(hrgn,0,NULL);
	CArray<BYTE,BYTE&> buffer;
	buffer.SetSize(sizeof(::RGNDATA) + dwSize);
	RGNDATA* pData = (RGNDATA*)buffer.GetData();
	if (::GetRegionData(hrgn,dwSize,pData) == 0) {
		ASSERT(FALSE);
		return NULL;
	}
	CPoint ptWOrg = pDC->GetWindowOrg();
	CSize  szWExt = pDC->GetWindowExt();
	CPoint ptVOrg = pDC->GetViewportOrg();
	CSize  szVExt = pDC->GetViewportExt();
	double xWV = static_cast<double>(szWExt.cx) / szVExt.cx;
	double yWV = static_cast<double>(szWExt.cy) / szVExt.cy;
	XFORM xform;
	xform.eM11 = xWV;
	xform.eM12 = 0;
	xform.eDx  = ptWOrg.x - ptVOrg.x * xWV;
	xform.eM21 = 0;
	xform.eM22 = yWV;
	xform.eDy  = ptWOrg.y - ptVOrg.y * yWV;
	HRGN hrgnDst = ::ExtCreateRegion(&xform,dwSize,pData);
	ASSERT(hrgnDst != NULL);
	return hrgnDst;
}

void ScrollViewEx::LPtoDP(CDC* pDC,LPPOINT pPt,int nCount) {
	CPoint ptWin = pDC->GetWindowOrg();
	CPoint ptViw = pDC->GetViewportOrg();
	CSize sizeWinExt = pDC->GetWindowExt();
	CSize sizeVpExt  = pDC->GetViewportExt();
	while (0 < nCount--) {
		ASSERT(AfxIsValidAddress(pPt, sizeof(POINT)));
		pPt->x = double_2_int(static_cast<double>(pPt->x - ptWin.x) * sizeVpExt.cx / sizeWinExt.cx + ptViw.x);
		pPt->y = double_2_int(static_cast<double>(pPt->y - ptWin.y) * sizeVpExt.cy / sizeWinExt.cy + ptViw.y);
		pPt++;
	}
}

void ScrollViewEx::DPtoLP(CDC* pDC,LPPOINT pPt,int nCount) {
	CPoint ptWin = pDC->GetWindowOrg();
	CPoint ptViw = pDC->GetViewportOrg();
	CSize sizeWinExt = pDC->GetWindowExt();
	CSize sizeVpExt  = pDC->GetViewportExt();
	while (0 < nCount--) {
		ASSERT(AfxIsValidAddress(pPt, sizeof(POINT)));
		pPt->x = double_2_int(static_cast<double>(pPt->x - ptViw.x) * sizeWinExt.cx / sizeVpExt.cx + ptWin.x);
		pPt->y = double_2_int(static_cast<double>(pPt->y - ptViw.y) * sizeWinExt.cy / sizeVpExt.cy + ptWin.y);
		pPt++;
	}
}

void ScrollViewEx::LPtoDP(CDC* pDC,LPSIZE lpSize) {
	ASSERT(AfxIsValidAddress(lpSize, sizeof(SIZE)));

	CSize sizeWinExt = pDC->GetWindowExt();
	CSize sizeVpExt  = pDC->GetViewportExt();
	lpSize->cx = double_2_int(static_cast<double>(lpSize->cx) * abs(sizeVpExt.cx) / abs(sizeWinExt.cx));
	lpSize->cy = double_2_int(static_cast<double>(lpSize->cy) * abs(sizeVpExt.cy) / abs(sizeWinExt.cy));
}

void ScrollViewEx::DPtoLP(CDC* pDC,LPSIZE lpSize) {
	ASSERT(AfxIsValidAddress(lpSize, sizeof(SIZE)));

	CSize sizeWinExt = pDC->GetWindowExt();
	CSize sizeVpExt  = pDC->GetViewportExt();
	lpSize->cx = double_2_int(static_cast<double>(lpSize->cx) * abs(sizeWinExt.cx) / abs(sizeVpExt.cx));
	lpSize->cy = double_2_int(static_cast<double>(lpSize->cy) * abs(sizeWinExt.cy) / abs(sizeVpExt.cy));
}

void ScrollViewEx::LPtoDP(CDC* pDC,LPRECT lpRect) {
	ASSERT(AfxIsValidAddress(lpRect, sizeof(RECT)));
	POINT pt = {lpRect->left,lpRect->top}; 
	SIZE  sz = {lpRect->right - lpRect->left,lpRect->bottom - lpRect->top}; 
	LPtoDP(pDC,&pt);
	LPtoDP(pDC,&sz);
	::SetRect(lpRect,pt.x,pt.y,pt.x + sz.cx,pt.y + sz.cy);
}

void ScrollViewEx::DPtoLP(CDC* pDC,LPRECT lpRect) {
	ASSERT(AfxIsValidAddress(lpRect, sizeof(RECT)));
	POINT pt = {lpRect->left,lpRect->top}; 
	SIZE  sz = {lpRect->right - lpRect->left,lpRect->bottom - lpRect->top}; 
	DPtoLP(pDC,&pt);
	DPtoLP(pDC,&sz);
	::SetRect(lpRect,pt.x,pt.y,pt.x + sz.cx,pt.y + sz.cy);
}

void ScrollViewEx::ScrollToCenter() {
	ScrollToCenter(CPoint(0,0));
}

inline int limit(int n,int min,int max) {
	return min(max(n,min),max);
}

void ScrollViewEx::ScrollToCenter(CPoint ptLP) {
	CRect rc;
	GetClientRect(rc);
	CSize szLP = rc.Size() / 2;
	DPtoLP(&szLP);
	ScrollToPosition(ptLP - szLP);
}

CPoint ScrollViewEx::GetCenterScrollPosition() const {
	CRect rc;
	GetClientRect(rc);
	CPoint ptLP = rc.Size() / 2;
	DPtoLP(&ptLP);

	if (!(GetStyle() & WS_HSCROLL)) {
		ptLP.x = 0;
	}
	if (!(GetStyle() & WS_VSCROLL)) {
		ptLP.y = 0;
	}

	return ptLP;
}

void ScrollViewEx::ScrollToPosition(CPoint pt) {
	if (m_nMapMode == MM_SCALE ||
		       m_nMapMode == MM_SCALE_DOC2FIT ||
			   m_nMapMode == MM_SCALE_TOTAL2FIT ) {
		LPtoDP(&pt);
		pt.x += GetScrollPos(SB_HORZ);
		pt.y += GetScrollPos(SB_VERT);
	} else {
		ASSERT(FALSE);
	}

	// now in device coordinates - limit if out of range
	int xMax = GetScrollLimit(SB_HORZ);
	int yMax = GetScrollLimit(SB_VERT);
	if (pt.x < 0)
		pt.x = 0;
	else if (pt.x > xMax)
		pt.x = xMax;
	if (pt.y < 0)
		pt.y = 0;
	else if (pt.y > yMax)
		pt.y = yMax;

	ScrollToDevicePosition(pt);
}

CPoint ScrollViewEx::GetDeviceScrollPosition() const {
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);

	if (m_bCenter) {
		CRect rc;
		GetClientRect(rc);
		CSize sz = rc.Size();
		if (m_totalDev.cx < sz.cx)
			pt.x = -((sz.cx - m_totalDev.cx) / 2);
		if (m_totalDev.cy < sz.cy)
			pt.y = -((sz.cy - m_totalDev.cy) / 2);
	}

	return pt;
}


void ScrollViewEx::ScrollToDevicePosition(POINT ptDev)
{
	ASSERT(ptDev.x >= 0);
	ASSERT(ptDev.y >= 0);

	// Note: ScrollToDevicePosition can and is used to scroll out-of-range
	//  areas as far as ScrollViewEx is concerned -- specifically in
	//  the print-preview code.  Since OnScrollBy makes sure the range is
	//  valid, ScrollToDevicePosition does not vector through OnScrollBy.

	int xOrig = GetScrollPos(SB_HORZ);
	SetScrollPos(SB_HORZ, ptDev.x);
	int yOrig = GetScrollPos(SB_VERT);
	SetScrollPos(SB_VERT, ptDev.y);
	ScrollWindow(xOrig - ptDev.x, yOrig - ptDev.y);
	Invalidate();
}

inline void FillNoEmptyRect(CDC* pDC,const CRect& _rc,CBrush* pBrush) {
	CRect rc(_rc);
	if (rc.right < rc.left) {
		rc.right = rc.left;
	}
	if (rc.bottom < rc.top) {
		rc.bottom = rc.top;
	}
	if (rc.IsRectEmpty())
		return;
	pDC->FillRect(rc,pBrush);
}

inline CPoint TopRight(const CRect& rc) {
	return CPoint(rc.right,rc.top);
}

inline CPoint BottomLeft(const CRect& rc) {
	return CPoint(rc.left,rc.bottom);
}

void ScrollViewEx::GetDocumentRect(LPRECT prc) const {
	CRect rcDocument(-m_documentCenter,m_documentLog);
	LPtoDP(rcDocument);
	*prc = rcDocument;
}

void ScrollViewEx::GetScrollableRect(LPRECT prc) const {
	CRect rcTotal(-((m_totalLog - m_documentLog) / 2 + m_documentCenter),m_totalLog);
	LPtoDP(rcTotal);
	*prc = rcTotal;
}

BOOL ScrollViewEx::PtInDocumentRect(CPoint devpt) const {
	CRect rc;
	GetDocumentRect(rc);
	return rc.PtInRect(devpt);
}

BOOL ScrollViewEx::PtInScrollableRect(CPoint devpt) const {
	CRect rc;
	GetScrollableRect(rc);
	return rc.PtInRect(devpt);
}

void ScrollViewEx::ResizeParentToFit(BOOL bShrinkOnly) {
	ASSERT(m_nMapMode != MM_NONE);

	CRect rectClient;
	GetWindowRect(rectClient);
	CRect rect = rectClient;
	CalcWindowRect(rect);
	rectClient.left += rectClient.left - rect.left;
	rectClient.top += rectClient.top - rect.top;
	rectClient.right -= rect.right - rectClient.right;
	rectClient.bottom -= rect.bottom - rectClient.bottom;
	rectClient.OffsetRect(-rectClient.left, -rectClient.top);
	ASSERT(rectClient.left == 0 && rectClient.top == 0);

	CRect rectView(0, 0, m_totalDev.cx, m_totalDev.cy);
	if (bShrinkOnly) {
		if (rectClient.right <= m_totalDev.cx)
			rectView.right = rectClient.right;
		if (rectClient.bottom <= m_totalDev.cy)
			rectView.bottom = rectClient.bottom;
	}
	CalcWindowRect(rectView, CWnd::adjustOutside);
	rectView.OffsetRect(-rectView.left, -rectView.top);
	ASSERT(rectView.left == 0 && rectView.top == 0);
	if (bShrinkOnly) {
		if (rectClient.right <= m_totalDev.cx)
			rectView.right = rectClient.right;
		if (rectClient.bottom <= m_totalDev.cy)
			rectView.bottom = rectClient.bottom;
	}


	CRect rectFrame;
	CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);
	pFrame->GetWindowRect(rectFrame);
	CSize size = rectFrame.Size();
	size.cx += rectView.right - rectClient.right;
	size.cy += rectView.bottom - rectClient.bottom;
	pFrame->SetWindowPos(NULL, 0, 0, size.cx, size.cy,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
}

void ScrollViewEx::GetScrollBarSizes(CSize& sizeSb) const {
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = GetStyle();

	if (GetScrollBarCtrl(SB_VERT) == NULL)
	{
		// vert scrollbars will impact client area of this window
		sizeSb.cx = safxData.cxVScroll;
		if (dwStyle & WS_BORDER)
			sizeSb.cx -= CX_BORDER;
	}
	if (GetScrollBarCtrl(SB_HORZ) == NULL)
	{
		// horz scrollbars will impact client area of this window
		sizeSb.cy = safxData.cyHScroll;
		if (dwStyle & WS_BORDER)
			sizeSb.cy -= CY_BORDER;
	}
}

BOOL ScrollViewEx::GetTrueClientSize(CSize& size, CSize& sizeSb) const {
	CRect rect;
	GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes(sizeSb);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// vert scrollbars will impact client area of this window
		size.cx += sizeSb.cx;   // currently on - adjust now
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// horz scrollbars will impact client area of this window
		size.cy += sizeSb.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

void ScrollViewEx::GetScrollBarState(CSize sizeClient, CSize& needSb,
	CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient) const
{
	// get scroll bar sizes (the part that is in the client area)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// enough room to add scrollbars
	sizeRange = m_totalDev - sizeClient;
		// > 0 => need to scroll
	ptMove = GetDeviceScrollPosition();
		// point to move to (start at current scroll pos)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// need a horizontal scrollbar after all
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// if current scroll position will be past the limit, scroll to limit
	if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
		ptMove.x = sizeRange.cx;
	if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
		ptMove.y = sizeRange.cy;

	// now update the bars as appropriate
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, and ptMove area now all updated
}

void ScrollViewEx::UpdateBars()
{
	// UpdateBars may cause window to be resized - ignore those resizings
	if (m_bInsideUpdate)
		return;         // Do not allow recursive calls

	// Lock out recursion
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	// allow parent to do inside-out layout first
	CWnd* pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		// if parent window responds to this message, use just
		//  client area for scroll bar calc -- not "true" client area
		if ((BOOL)pParentWnd->SendMessage(WM_RECALCPARENT, 0,
			(LPARAM)(LPCRECT)&rectClient) != 0)
		{
			// use rectClient instead of GetTrueClientSize for
			//  client size calculation.
			bCalcClient = FALSE;
		}
	}

	CSize sizeClient;
	CSize sizeSb;

	if (bCalcClient)
	{
		// get client rect
		if (!GetTrueClientSize(sizeClient, sizeSb))
		{
			// no room for scroll bars (common for zero sized elements)
			CRect rect;
			GetClientRect(&rect);
			if (rect.right > 0 && rect.bottom > 0)
			{
				// if entire client area is not invisible, assume we have
				//  control over our scrollbars
				EnableScrollBarCtrl(SB_BOTH, FALSE);
			}
			m_bInsideUpdate = FALSE;
			return;
		}
	}
	else
	{
		// let parent window determine the "client" rect
		GetScrollBarSizes(sizeSb);
		sizeClient.cx = rectClient.right - rectClient.left;
		sizeClient.cy = rectClient.bottom - rectClient.top;
	}

	// enough room to add scrollbars
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;

	// get the current scroll bar state given the true client area
	GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
	if (needSb.cx)
		sizeClient.cy -= sizeSb.cy;
	if (needSb.cy)
		sizeClient.cx -= sizeSb.cx;

	// first scroll the window as needed
	ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

	// this structure needed to update the scrollbar page range
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// now update the bars as appropriate
	EnableScrollBarCtrl(SB_HORZ, needSb.cx != 0);
	if (needSb.cx) {
		info.nPage = sizeClient.cx;
		info.nMax = m_totalDev.cx-1;
		if (!SetScrollInfo(SB_HORZ, &info, TRUE))
			SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
	}

	EnableScrollBarCtrl(SB_VERT, needSb.cy != 0);
	if (needSb.cy) {
		info.nPage = sizeClient.cy;
		info.nMax = m_totalDev.cy-1;
		if (!SetScrollInfo(SB_VERT, &info, TRUE))
			SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
	}

	// remove recursion lockout
	m_bInsideUpdate = FALSE;
}

void ScrollViewEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;
	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		return;

	OnScroll(MAKEWORD(nSBCode, -1), nPos);
}

void ScrollViewEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	OnScroll(MAKEWORD(-1, nSBCode), nPos);
}

BOOL ScrollViewEx::OnMouseWheel(UINT fFlags, short zDelta, CPoint point) {
	if (fFlags & (MK_SHIFT | MK_CONTROL))
		return FALSE;

	if (GetParentSplitter(this, TRUE))
		return FALSE;

	return DoMouseWheel(fFlags, zDelta, point);
}

BOOL ScrollViewEx::DoMouseWheel(UINT fFlags, short zDelta, CPoint point) {
	UNUSED_ALWAYS(point);
	UNUSED_ALWAYS(fFlags);

	DWORD dwStyle = GetStyle();
	CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
	BOOL bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_VSCROLL);

	pBar = GetScrollBarCtrl(SB_HORZ);
	BOOL bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_HSCROLL);

	if (!bHasVertBar && !bHasHorzBar)
		return FALSE;

	BOOL bResult = FALSE;
	UINT uWheelScrollLines = _AfxGetMouseScrollLines();
	int nToScroll;
	int nDisplacement;

	if (bHasVertBar)
	{
		nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
		if (nToScroll == -1 || uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = m_pageDev.cy;
			if (zDelta > 0)
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement = nToScroll * m_lineDev.cy;
			nDisplacement = min(nDisplacement, m_pageDev.cy);
		}
		bResult = OnScrollBy(CSize(0, nDisplacement), TRUE);
	}
	else if (bHasHorzBar)
	{
		nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
		if (nToScroll == -1 || uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = m_pageDev.cx;
			if (zDelta > 0)
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement = nToScroll * m_lineDev.cx;
			nDisplacement = min(nDisplacement, m_pageDev.cx);
		}
		bResult = OnScrollBy(CSize(nDisplacement, 0), TRUE);
	}

	if (bResult)
		UpdateWindow();

	return bResult;
}


BOOL ScrollViewEx::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) {
	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= m_lineDev.cx;
		break;
	case SB_LINEDOWN:
		x += m_lineDev.cx;
		break;
	case SB_PAGEUP:
		x -= m_pageDev.cx;
		break;
	case SB_PAGEDOWN:
		x += m_pageDev.cx;
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
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
		y -= m_lineDev.cy;
		break;
	case SB_LINEDOWN:
		y += m_lineDev.cy;
		break;
	case SB_PAGEUP:
		y -= m_pageDev.cy;
		break;
	case SB_PAGEDOWN:
		y += m_pageDev.cy;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}

	BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll) {
		Invalidate(FALSE);
		UpdateWindow();
	}

	return bResult;
}

BOOL ScrollViewEx::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) {
	int xOrig, x;
	int yOrig, y;

	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
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

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		ScrollWindow(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
			SetScrollPos(SB_HORZ, x);
		if (y != yOrig)
			SetScrollPos(SB_VERT, y);
	}
	return TRUE;
}

void ScrollViewEx::OnPaint() {
	CPaintDC dcPaint(this);
	CRect rc;
	GetClientRect(rc);

	if (m_nMapMode == MM_NONE) {
		dcPaint.FillSolidRect(rc,::GetSysColor(COLOR_BTNFACE));
		return;
	}

	BOOL bUseMem = TRUE;
	CDC dcMem;
	CBitmap bmp;
	CBitmap* pbmpOld = NULL;
	if (bUseMem) {
		bUseMem = dcMem.CreateCompatibleDC(&dcPaint);
		ASSERT(bUseMem);
	}
	if (bUseMem) {
		bUseMem = bmp.CreateCompatibleBitmap(&dcPaint,rc.Width(),rc.Height());
		ASSERT(bUseMem);
	}
	if (bUseMem) {
		VERIFY(dcMem.SelectObject(&bmp) != NULL);
	}

	CDC* pDC = bUseMem ? &dcMem : &dcPaint;
	if (bUseMem) {
		dcMem.m_hAttribDC = dcPaint;
	}
	OnPrepareDC(pDC);
	OnDraw(pDC);

	if (bUseMem) {
		dcMem.SetViewportOrg(0,0);
		dcMem.SetWindowExt(rc.Size());
		dcMem.SetViewportExt(rc.Size());
		dcPaint.BitBlt(rc.left, rc.top,rc.Width(),rc.Height(),&dcMem,rc.left,rc.top,SRCCOPY);
		dcMem.SelectObject(pbmpOld);
	}
}

BOOL ScrollViewEx::OnEraseBkgnd(CDC* pDC) {
	return TRUE;	
}

void ScrollViewEx::DrawDocumentBorder(CDC* pDC,CBrush* pBrush) const {
	CSetDCtoDP setDCtoDP(pDC);
	ASSERT_VALID(pBrush);

	CRect rc;
	GetDocumentRect(rc);
	pDC->FrameRect(rc,&CBrush(RGB(0x00,0x00,0x00)));
}

void ScrollViewEx::MaskDocumentRect(CDC* pDC,BOOL b) const {
	ASSERT_VALID(pDC);
	if (!b) {
		pDC->SelectClipRgn(NULL);
		return;
	}
	
	CRect rcDocument;
	GetDocumentRect(rcDocument);

	CRgn rgnMask;
	if (rgnMask.CreateRectRgnIndirect(rcDocument) == ERROR) {
		ASSERT(FALSE);
		return;
	}
	pDC->SelectClipRgn(&rgnMask,RGN_COPY);
}

void ScrollViewEx::FillBkgnd(CDC* pDC, CBrush* pBrush) const {
	ASSERT_VALID(pBrush);
	CSetDCtoDP setDCtoDP(pDC);

	CRect	rect;
	GetClientRect(&rect);

	pDC->FillRect(rect,pBrush);
}

void ScrollViewEx::FillDocumentRect(CDC* pDC,CBrush* pBrush) const {
	ASSERT_VALID(pBrush);
	CSetDCtoDP setDCtoDP(pDC);

	CRect rc;
	GetDocumentRect(rc);

	pDC->FillRect(rc,pBrush);
}

void ScrollViewEx::FillScrollabeRect(CDC* pDC,CBrush* pBrush) const {
	CRect rc(-((m_totalLog - m_documentLog) / 2 + m_documentCenter),m_totalLog);
	pDC->FillRect(rc,pBrush);
}

void ScrollViewEx::DrawCenterLine(CDC* pDC,CPen& pen) {
	ASSERT_VALID(&pen);
	CSetDCtoDP setDCtoDP(pDC);

	CRect	rect;
	GetClientRect(&rect);
	CPoint  ptDocCenter(0,0);
	ScrollViewEx::LPtoDP(&ptDocCenter);

	pDC->MoveTo(ptDocCenter.x,rect.top);
	pDC->LineTo(ptDocCenter.x,rect.bottom);
	pDC->MoveTo(rect.left, ptDocCenter.y);
	pDC->LineTo(rect.right,ptDocCenter.y);
}

void ScrollViewEx::DrawGridHelper(CDC *pDC,CPen& pen,int nInterval) {
	ASSERT_VALID(pDC);
	ASSERT_VALID(&pen);
	ASSERT(0 < nInterval);
	CPen *pOldPen = (CPen*)pDC->SelectObject(&pen);
	ASSERT(pOldPen != NULL);

	CRect	rect;
	GetClientRect(&rect);
	ScrollViewEx::DPtoLP(rect);

	int xstart = (nInterval == 1) ? rect.left : (rect.left - rect.left % nInterval);
	int ystart = (nInterval == 1) ? rect.top  : (rect.top  - rect.top  % nInterval);
	int xend = (nInterval == 1) ? rect.right  : (nInterval - rect.right  % nInterval + rect.right);
	int yend = (nInterval == 1) ? rect.bottom : (nInterval - rect.bottom % nInterval + rect.bottom);

	for(int x = xstart;x < xend;x += nInterval) {
		pDC->MoveTo(x,rect.top);
		pDC->LineTo(x,rect.bottom);
	}
	for(int y = ystart;y < yend;y += nInterval) {
		pDC->MoveTo(rect.left, y);
		pDC->LineTo(rect.right,y);
	}
	pDC->SelectObject(pOldPen);
}

void ScrollViewEx::OnInitialUpdate() {
	ASSERT(!m_bInitialUpdated);
	CView::OnInitialUpdate();
	m_bInitialUpdated = TRUE; 
}

void ScrollViewEx::OnUpdateScale(CCmdUI* pCmdUI) {
	ASSERT(pCmdUI != NULL);
	HWND hwnd = (HWND) pCmdUI->m_pOther->m_hWnd;
	CWnd* pWnd = CWnd::FromHandle(hwnd);

	GETSCALEPARAM param;
	param.m_scale = GetScale();

	if (MM_SCALE_DOC2FIT == m_nMapMode) {
		param.m_typeGetScale = getScaleDocumentToFitSize;
	} else if (MM_SCALE_TOTAL2FIT == m_nMapMode) {
		param.m_typeGetScale = getScaleTotalToFitSize;
	} else { 
		ASSERT(m_nMapMode == MM_SCALE);
		param.m_typeGetScale = getScale;
	}
	pWnd->SendMessage(WM_SCROLLVIEWEX_GETSCALE,0,(LPARAM)&param);
}

void ScrollViewEx::OnSize(UINT nType, int cx, int cy) {
	CView::OnSize(nType, cx, cy);
	if (m_nMapMode == MM_SCALE_DOC2FIT) {
		SetScaleDocumentToFitSize();
	} else if (m_nMapMode == MM_SCALE_TOTAL2FIT) {
		SetScaleTotalToFitSize();
	} else if (m_nMapMode != MM_NONE){
		UpdateBars();
		Invalidate();
	}
}

void ScrollViewEx::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) {
	if (m_nMapMode == MM_NONE)
		return;

	ASSERT_VALID(pDC);
	ASSERT(m_nMapMode != MM_NONE);
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
	
	switch (m_nMapMode) {
	case MM_SCALE:
	case MM_SCALE_DOC2FIT:
	case MM_SCALE_TOTAL2FIT:
		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowExt(m_totalLog);  // window is in logical coordinates
		pDC->SetViewportExt(m_totalLog * m_scale /*CalcScale(m_totalLog,m_totalDev)*/);
		ASSERT(!(m_totalDev.cx == 0 || m_totalDev.cy == 0));
		break;
	default:
		ASSERT(m_nMapMode > 0);
		pDC->SetMapMode(m_nMapMode);
		break;
	}

	CPoint ptVpOrg(0, 0);
	if (!pDC->IsPrinting()) {
		ASSERT(pDC->GetWindowOrg() == CPoint(0,0));
		ptVpOrg = -GetDeviceScrollPosition();
		double dScale = static_cast<double>(m_totalDev.cx) / m_totalLog.cx;
		const CPoint ptDocCenter = m_documentCenter * dScale;
		const CSize docDev = m_documentLog    * dScale;
		ptVpOrg += (m_totalDev - docDev) / 2 + ptDocCenter;
		
		if (m_bCenter) {
			CRect rc;
			GetClientRect(rc);
			CSize sz = rc.Size();
			if (m_totalDev.cx < sz.cx)
				ptVpOrg.x = (sz.cx - docDev.cx) / 2 + ptDocCenter.x;
			if (m_totalDev.cy < sz.cy)
				ptVpOrg.y = (sz.cy - docDev.cy) / 2 + ptDocCenter.y;
		}
	}
	pDC->SetViewportOrg(ptVpOrg);
	CView::OnPrepareDC(pDC, pInfo);
}

void ScrollViewEx::OnScale() {
	MSG& msg = AfxGetApp()->m_msgCur;
	CWnd* pWnd = CWnd::FromHandle((HWND)msg.lParam);
	ASSERT_VALID(pWnd);
	SETSCALEPARAM param;
	pWnd->SendMessage(WM_SCROLLVIEWEX_SETSCALE,0,(LPARAM)&param);

	switch(param.m_typeSetScale) {
	case setScale:
		SetScale(param.m_scale);
		break;
	case setScaleDocumentToFitSize:
		SetScaleDocumentToFitSize();
		break;
	case setScaleTotalToFitSize:
		SetScaleTotalToFitSize();
		break;
	case setResizeParentToFit:
		ResizeParentToFit();
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	Invalidate();
}

CPoint ScrollViewEx::GetScrollPosition() const {
	if (m_nMapMode == MM_SCALE_TOTAL2FIT) {
		return CPoint(0, 0);
	}

	CPoint pt = GetDeviceScrollPosition();
	DPtoLP(&pt);
	return pt;
}

void ScrollViewEx::SetScaleTotalToFitSize() {
	m_nMapMode = MM_SCALE_TOTAL2FIT;
	UpdateScrollSizes();
}

void ScrollViewEx::SetScaleDocumentToFitSize() {
	m_nMapMode = MM_SCALE_DOC2FIT;
	UpdateScrollSizes();
}
 
double ScrollViewEx::CalcScale(CSize szSrc,CSize szDst) {
	double xs = static_cast<double>(szDst.cx) / szSrc.cx;
	double ys = static_cast<double>(szDst.cy) / szSrc.cy;
	ys = std::_MAX(MIN_SCALE,ys);
	xs = std::_MAX(MIN_SCALE,xs);
	return std::_MIN(xs,ys);
}

void ScrollViewEx::UpdateScrollSizes() {
	if (m_bUpdateScrollSizes)
		return;
	m_bUpdateScrollSizes = TRUE;

	BOOL bToScroll  = FALSE;
	CPoint ptToScroll;
	if (m_totalDev.cx != 0 && m_totalDev.cy != 0) {
		ptToScroll = GetCenterScrollPosition();
	} else {
		ptToScroll.x = 0;
		ptToScroll.y = 0;
	}

	if (m_nMapMode == MM_SCALE_TOTAL2FIT) {
		if (m_hWnd != NULL && (GetStyle() & (WS_HSCROLL|WS_VSCROLL))) {
			SetScrollPos(SB_HORZ, 0);
			SetScrollPos(SB_VERT, 0);
			EnableScrollBarCtrl(SB_BOTH, FALSE);
		}

		CRect rectT;
		GetClientRect(rectT);
		m_scale = CalcScale(m_totalLog,rectT.Size());
		ASSERT(0 < m_scale);
		
	} else if (m_nMapMode == MM_SCALE_DOC2FIT) {
		CRect rectT;
		GetWindowRect(rectT);
		CSize sz = rectT.Size();
		CSize szSb;
		GetScrollBarSizes(szSb);
		if (sz.cx < m_totalLog.cx)
			sz.cy -= szSb.cy;
		if (sz.cy < m_totalLog.cy)
			sz.cx -= szSb.cx;
		m_scale = CalcScale(m_documentLog,sz);
		ASSERT(0 < m_scale);
		ptToScroll = CPoint(0,0);
		bToScroll  = TRUE;

	} else if (m_nMapMode == MM_SCALE) {
		bToScroll  = TRUE;

	} else {
		ASSERT(FALSE);
	}

	m_totalDev = m_totalLog * m_scale;
	if (m_hWnd != NULL) {
		UpdateBars();
		CRect rc;
		GetClientRect(rc);
		CSize sz = rc.Size() / 2;
		m_pageDev.cx = max(sz.cx,1);
		m_pageDev.cy = max(sz.cy,1);
		m_lineDev.cx = m_lineDev.cy = max(1 * m_scale,1);
		Invalidate();
		if (bToScroll) {
			ScrollToCenter(ptToScroll);
		}
	}

	m_bUpdateScrollSizes = FALSE;
}

void ScrollViewEx::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) {
	if (nAdjustType == adjustOutside) {
		::AdjustWindowRectEx(lpClientRect, 0, FALSE, GetExStyle());

		if (m_nMapMode != MM_SCALE_TOTAL2FIT && m_nMapMode != MM_SCALE_DOC2FIT) {
			// if the view is being used in-place, add scrollbar sizes
			//  (scollbars should appear on the outside when in-place editing)
			CSize sizeClient(
				lpClientRect->right - lpClientRect->left,
				lpClientRect->bottom - lpClientRect->top);

			CSize sizeRange = m_totalDev - sizeClient;
				// > 0 => need to scroll

			// get scroll bar sizes (used to adjust the window)
			CSize sizeSb;
			GetScrollBarSizes(sizeSb);

			// adjust the window size based on the state
			if (sizeRange.cy > 0)
			{   // vertical scroll bars take up horizontal space
				lpClientRect->right += sizeSb.cx;
			}
			if (sizeRange.cx > 0)
			{   // horizontal scroll bars take up vertical space
				lpClientRect->bottom += sizeSb.cy;
			}
		}
	} else {
		::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE,
			GetExStyle() & ~(WS_EX_CLIENTEDGE));
	}
}

void DrawCross(CDC& dc,CPoint pt)
{
	const int CROSS_LEN = 4;

	dc.MoveTo(pt.x - CROSS_LEN,pt.y);
	dc.LineTo(pt.x + CROSS_LEN,pt.y);

	dc.MoveTo(pt.x,pt.y - CROSS_LEN);
	dc.LineTo(pt.x,pt.y + CROSS_LEN);
}

void ScrollViewEx::DrawCenter(CDC* dc,CPoint pt) {
	int nSaved = dc->SaveDC();
	dc->SetMapMode(MM_TEXT);
	dc->SetWindowOrg(0,0);
	dc->SetViewportOrg(0,0);

	CPen penFore(PS_SOLID,0,RGB(0x00,0x00,0x00));
	CPen penShdw(PS_SOLID,0,RGB(0xFF,0xFF,0xFF));

	CPen* pen = dc->SelectObject(&penShdw);
	ASSERT_VALID(pen);
	DrawCross(*dc,pt + CPoint(1,1));
	
	dc->SelectObject(&penFore);
	DrawCross(*dc,pt);
	VERIFY(dc->SelectObject(pen) != NULL);

	VERIFY(dc->RestoreDC(nSaved));
}