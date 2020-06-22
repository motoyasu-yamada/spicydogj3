#include "stdafx.h"
#include "Tracker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
	ヘルパライブラリ
*/
inline int d2i(double d) { return static_cast<int>((0 < d) ? (d + 0.5) : (d - 0.5)); }

inline CSize operator * (const CSize& sz,double d) {
	return CSize(d2i(sz.cx * d),d2i(sz.cy * d));
}

inline CSize operator / (const CSize& sz,double d) {
	return CSize(d2i(sz.cx / d),d2i(sz.cy / d));
}

inline CSize operator * (double d,const CSize& sz) {
	return CSize(d2i(sz.cx * d),d2i(sz.cy * d));
}

inline CSize operator / (double d,const CSize& sz) {
	return CSize(d2i(sz.cx / d),d2i(sz.cy / d));
}

/// 頂点設定
inline void SetPoint(LPPOINT ppt,int x,int y) {
	ASSERT(AfxIsValidAddress(ppt,sizeof POINT));
	ppt->x = x;
	ppt->y = y;
}

/// 矩形のハンドル位置の取得
CPoint GetHandlePos(LPCRECT devRect,int nHandle) {
	ASSERT(AfxIsValidAddress(devRect,sizeof RECT));
	CPoint pt;
	switch(nHandle)
	{
	case CRectTracker::hitTopLeft:
	case CRectTracker::hitTopRight:
	case CRectTracker::hitTop:
		pt.y = devRect->top;
		break;
	case CRectTracker::hitBottomLeft:
	case CRectTracker::hitBottomRight:
	case CRectTracker::hitBottom:
		pt.y = devRect->bottom;
		break;
	case CRectTracker::hitLeft:
	case CRectTracker::hitRight:
		pt.y = (devRect->top + devRect->bottom) / 2;
		break;
	case CRectTracker::hitMiddle:
	default:
		ASSERT(FALSE);
		break;
	}

	switch(nHandle)
	{
	case CRectTracker::hitTopLeft:
	case CRectTracker::hitBottomLeft:
	case CRectTracker::hitLeft:
		pt.x = devRect->left;
		break;
	case CRectTracker::hitTopRight:
	case CRectTracker::hitBottomRight:
	case CRectTracker::hitRight:
		pt.x = devRect->right;
		break;
	case CRectTracker::hitTop:
	case CRectTracker::hitBottom:
		pt.x = (devRect->right + devRect->left) / 2;
		break;
	case CRectTracker::hitMiddle:
		ASSERT(FALSE);
		break;
	}
	return pt;
}

/// 原点を中心とした回転行列計算
inline void RotatePoint(double& x,double& y,double r) {
	const double dcos = cos(r);
	const double dsin = sin(r);
	const double _x = x;
	const double _y = y;
	x = (_x * dcos)  - (_y * dsin);
	y = (_y * dcos)  + (_x * dsin);
}

const double PI = 3.1415926535;
const double PI2 = PI * 2;

/// 角度(ラジアン)を -π 以上 π未満に正規化する
inline double NormalizeRadian(double r) {
	while (PI <= r) {
		r -= PI2;
	}
	while (r < -PI) {
		r += PI2;
	}
	return r;
}

/// 回転行列計算
inline void RotatePoint(POINT& pt,POINT ptCenter,double r) {
	r = NormalizeRadian(r);
	double x = pt.x - ptCenter.x;
	double y = pt.y - ptCenter.y;
	RotatePoint(x,y,r);
	pt.x = d2i(x + ptCenter.x);
	pt.y = d2i(y + ptCenter.y);
}

/// 原点からみた指定点の回転位置を求める
inline double CalcRotate(CPoint pt,CPoint ptCenter){
	CPoint ptDelta = pt - ptCenter;
	return atan2(ptDelta.y,ptDelta.x);
}

/// 中央位置を基準に矩形を設定する
inline void SetRectByCenter(CRect& rc,CPoint ptCenter,CSize sz) {
	ASSERT(AfxIsValidAddress(&rc,sizeof CRect));
	ASSERT(2 <= sz.cx && 2 <= sz.cy);

	rc.SetRect(ptCenter.x - sz.cx / 2,
		       ptCenter.y - sz.cy / 2,
               ptCenter.x + sz.cx / 2,
               ptCenter.y + sz.cy / 2);
}

inline void ScalePoint(POINT& _pt,CPoint ptCenter,double sx,double sy) {
	CPoint pt = _pt;
	pt -= ptCenter;
	pt.x = d2i(pt.x * sx);
	pt.y = d2i(pt.y * sy);
	pt += ptCenter;
	_pt =pt;
}

inline CPoint ConvertLParam2Point(LPARAM lpm) {
	return CPoint((int)(short)LOWORD(lpm),(int)(short)HIWORD(lpm));
}

inline double distance(double x,double y) {
	return pow(x * x + y * y,0.5);
}

inline double distance(CSize sz) {
	return distance(sz.cx,sz.cy);
}

/// 45度(象限の角度幅)
const double PSS = PI / 4;
const double RS0 = -PI + PSS / 2;
const double RS1 = RS0 + PSS;
const double RS2 = RS1 + PSS;
const double RS3 = RS2 + PSS;
const double RS4 = RS3 + PSS;
const double RS5 = RS4 + PSS;
const double RS6 = RS5 + PSS;
const double RS7 = RS6 + PSS;
const double RT0 = 0;
const double RT1 = RT0 + PSS;
const double RT2 = RT1 + PSS;
const double RT3 = RT2 + PSS;
const double RT4 = RT3 + PSS;
const double RT5 = RT4 + PSS;
const double RT6 = RT5 + PSS;
const double RT7 = RT6 + PSS;

/**
	角度の種類を求める(左・左上・上・右上・右・右下・下・左下)
	@param rotate ラジアン角度は正規化(-π～π)でないとならない
*/
int RadianType(double rotate) {
	ASSERT(RS7 < PI);
	ASSERT(RS3 < 0);
	ASSERT(0 < RS4);

	if (rotate <  RS0 || RS7 < rotate) {
		return CRectTracker::hitLeft;
	}
	if (rotate <= RS1) {
		return CRectTracker::hitTopLeft;
	}
	if (rotate <= RS2) {
		return CRectTracker::hitTop;
	}
	if (rotate <= RS3) {
		return CRectTracker::hitTopRight;
	}
	if (rotate <= RS4) {
		return CRectTracker::hitRight;
	}
	if (rotate <= RS5) {
		return CRectTracker::hitBottomRight;
	}
	if (rotate <= RS6) {
		return CRectTracker::hitBottom; 
	}
	ASSERT(rotate < RS7);
	return CRectTracker::hitBottomLeft;
}

inline double AdjustRotate(double r) {
	r = NormalizeRadian(r);

	switch(RadianType(r)) {
	case CRectTracker::hitLeft:
		return RT0;
	case CRectTracker::hitBottomLeft:
		return RT1;
	case CRectTracker::hitBottom:
		return RT2;
	case CRectTracker::hitBottomRight:
		return RT3;
	case CRectTracker::hitRight:
		return RT4;
	case CRectTracker::hitTopRight:
		return RT5;
	case CRectTracker::hitTop:
		return RT6;
	case CRectTracker::hitTopLeft:
		return RT7;
	default:
		ASSERT(FALSE);
		break;
	}
	return r;
}

LPCTSTR RectHandleType2Cursor(int n){
	switch(n) {
	case CRectTracker::hitTopLeft:
	case CRectTracker::hitBottomRight:
		return IDC_SIZENWSE;
	case CRectTracker::hitTopRight:
	case CRectTracker::hitBottomLeft:
		return IDC_SIZENESW;
	case CRectTracker::hitTop:
	case CRectTracker::hitBottom:
		return IDC_SIZENS;
	case CRectTracker::hitRight:
	case CRectTracker::hitLeft:
		return IDC_SIZEWE;
	default:
		ASSERT(FALSE);
		return 0;
	}
}

int CalcRectHandleType(CPoint pt,CPoint ptCenter) {
	const double rotate = NormalizeRadian(CalcRotate(pt,ptCenter));
	return RadianType(rotate);
}

inline BOOL IsStillKeyDown(int nKey) { return ::GetAsyncKeyState(nKey) & 0x8000;}

inline BOOL IsShifted() { return IsStillKeyDown(VK_SHIFT); }

/**
	トラッカー独自のメッセージポンプを実装するクラス
*/
class TrackerMsgPump {
private:
	BOOL m_bLoop;
	BOOL m_bCanceled;
public:
	CDC*  m_pDC;
	MSG   m_msg;
public:
	TrackerMsgPump();
	BOOL Track(CWnd*);
	void ExitLoop();
	void CancelLoop();
protected:
	virtual ~TrackerMsgPump();
	virtual void OnMouseMove() = 0;
	virtual void OnPreLoop() = 0;
	virtual void OnPostLoop() = 0;

	virtual BOOL IsTracked() const;
	virtual void OnLButtonUp();
	virtual void OnRButtonDown();
	virtual void OnKeyDown();
}; // class TrackerMsgPump

TrackerMsgPump::TrackerMsgPump()
:	m_pDC(NULL) {
}

TrackerMsgPump::~TrackerMsgPump() {
}

void TrackerMsgPump::ExitLoop() {
	ASSERT(m_bLoop);
	m_bLoop     = FALSE;
	m_bCanceled = FALSE;
}

void TrackerMsgPump::CancelLoop() {
	ASSERT(m_bLoop);
	m_bLoop     = FALSE;
	m_bCanceled = FALSE;
}

BOOL TrackerMsgPump::IsTracked() const { return TRUE; }
void TrackerMsgPump::OnLButtonUp() { ExitLoop();}
void TrackerMsgPump::OnRButtonDown() { CancelLoop();}
void TrackerMsgPump::OnKeyDown() { 
	if (m_msg.wParam == VK_ESCAPE) {
		CancelLoop();
	}
}

BOOL TrackerMsgPump::Track(CWnd* pWnd) {
	if (::GetCapture() != NULL) {
		return FALSE;
	}

	pWnd->SetCapture();
	if (pWnd != CWnd::GetCapture()) {
		ASSERT(FALSE);
		return FALSE;
	}

	m_bLoop = TRUE;
	CClientDC dc(pWnd);
	m_pDC = &dc;
	ASSERT_VALID(m_pDC);

	OnPreLoop();

	AfxLockTempMaps();
	while (m_bLoop) {
		VERIFY(::GetMessage(&m_msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd) {
			break;
		}
		switch (m_msg.message) {
		case WM_LBUTTONUP:
			OnLButtonUp();
			break;
		case WM_MOUSEMOVE:
			OnMouseMove();
			break;
		case WM_KEYDOWN:
			OnKeyDown();
			break;
		case WM_RBUTTONDOWN:
			OnRButtonDown();
			break;
		default:
			DispatchMessage(&m_msg);
			break;
		}
	}
	AfxUnlockTempMaps(FALSE);

	ReleaseCapture();
	OnPostLoop();
	
	if (m_bCanceled) {
		return FALSE;
	}
	return IsTracked();
}


/// メッセージをトラッキングクラスにディスパッチする
template <typename _Ttracker,typename _Tparam>
class TTrackerMsgPump : public TrackerMsgPump {
private:
	_Ttracker& m_tracker;
	_Tparam&   m_param;
public:
	TTrackerMsgPump(_Ttracker& v,_Tparam& p)
	:	m_tracker(v),
		m_param(p) {
	}
protected:
	virtual void OnMouseMove(){ m_tracker.OnMouseMove(this,m_param);}
	virtual void OnPreLoop()  { m_tracker.DrawTracking(m_pDC); }
	virtual void OnPostLoop() { m_tracker.DrawTracking(m_pDC); } 
};

ShiftRectTracker::ShiftRectTracker(ADJUSTFUNC func,UINT nMask)
:	m_funcAdjust(func),
	m_nMask(nMask),
	m_szTrackBorder(1,1) {
}

BOOL ShiftRectTracker::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert) {
	m_rcOrg = m_rect;
	return CRectTracker::Track(pWnd,point,bAllowInvert);
}

UINT ShiftRectTracker::GetHandleMask() const {
	return m_nMask;
}

BOOL ShiftRectTracker::IsShifted() const {
	return ::IsShifted();
}

void ShiftRectTracker::AdjustRect(int nHandle,LPRECT lpRect) {
	if (!IsShifted()) {
		CRectTracker::AdjustRect(nHandle,lpRect);
	} else {
		m_funcAdjust(nHandle,lpRect,&m_rcOrg);
	}
}

void ShiftRectTracker::SetTrackerBorder(CSize sz) {
	m_szTrackBorder = sz;
}

void ShiftRectTracker::DrawTrackerRect(LPCRECT lpRect,CWnd* pWndClipTo,CDC* pDC,CWnd* pWnd) {
	CRect rect = *lpRect;
	rect.NormalizeRect();

	if (pWndClipTo != NULL) {
		pWnd->ClientToScreen(&rect);
		pWndClipTo->ScreenToClient(&rect);
	}

	CSize size(0, 0);
	if (!m_bFinalErase) {
		if (m_nStyle & hatchedBorder) {
			size.cx = size.cy = max(1, GetHandleSize(rect)-1);
			rect.InflateRect(size);
		} else {
			size = m_szTrackBorder;
		}
	}

	if (m_bFinalErase || !m_bErase)
		pDC->DrawDragRect(rect, size, m_rectLast, m_sizeLast);
	m_rectLast = rect;
	m_sizeLast = size;
}

void AdjustMove(POINT& ptCur,POINT ptOrg) {
	ASSERT(AfxIsValidAddress(&ptCur,sizeof POINT));

	const int ox = abs(ptCur.x - ptOrg.x); 
	const int oy = abs(ptCur.y - ptOrg.y);
	if (ox < oy) {
		ptCur.x  = ptOrg.x;
	} else if (oy < ox) {
		ptCur.y  = ptOrg.y;
	}
}

void AdjustRectMove(int nHandle,LPRECT pCur,LPCRECT pOrg) {
	ASSERT(AfxIsValidAddress(pCur,sizeof RECT));
	ASSERT(AfxIsValidAddress(pOrg,sizeof RECT));
	ASSERT((pCur->bottom - pCur->top ) == (pOrg->bottom - pOrg->top));
	ASSERT((pCur->right  - pCur->left) == (pOrg->right  - pOrg->left));

	int ox = abs(pCur->left - pOrg->left); 
	int oy = abs(pCur->top  - pOrg->top);

	if (ox < oy) {
		pCur->left  = pOrg->left;
		pCur->right = pOrg->right;
	} else if (oy < ox) {
		pCur->top    = pOrg->top;
		pCur->bottom = pOrg->bottom;
	}
}

MoveTracker::MoveTracker()
:	ShiftRectTracker(AdjustRectMove,0) {
}

BOOL MoveTracker::TrackMove(CWnd* pWnd,CPoint point) {
	return ShiftRectTracker::Track(pWnd,point);
}

void AdjustRectResize (int nHandle,LPRECT pCur,LPCRECT pOrg) {
	CRect rcOrg(pOrg);
	CRect rc   (pCur);
	
	int nZoom = ::MulDiv(rc.Width(),100,rcOrg.Width());

	int nWidth  = MulDiv(rcOrg.Width() ,  nZoom,100); 
	int nHeight = MulDiv(rcOrg.Height() , nZoom,100); 
	switch(nHandle) {
	case CRectTracker::hitTopLeft:
		rc.left = rc.right  - nWidth;
		rc.top  = rc.bottom - nHeight;
		break;

	case CRectTracker::hitTopRight:
		rc.right = rc.left + nWidth;
		rc.top   = rc.bottom - nHeight;
		break;

	case CRectTracker::hitBottomLeft:
		rc.left = rc.right - nWidth;
		rc.bottom = rc.top + nHeight;
		break;

	case CRectTracker::hitBottomRight:
		rc.right = rc.left + nWidth;
		rc.bottom = rc.top + nHeight;
		break;

	case CRectTracker::hitMiddle:
	case CRectTracker::hitTop:
	case CRectTracker::hitBottom:
	case CRectTracker::hitLeft:
	case CRectTracker::hitRight:
		break;
	}

	*pCur = rc;
}


ResizeTracker::ResizeTracker()
:	ShiftRectTracker(AdjustRectResize,255) {
}

BOOL ResizeTracker::TrackResize(CWnd* pWnd,CPoint point) {
	ASSERT_VALID(pWnd);
	int n = HitTest(point);	
	if (n == CRectTracker::hitMiddle || n == CRectTracker::hitNothing)
		return FALSE;
	return ShiftRectTracker::Track(pWnd,point);
}

inline int SignAbs(int nSign,int nAbs) {
	ASSERT(0 <= nAbs);
	return (nSign < 0) ? (-nAbs) : nAbs;
}

void AdjustRectCreate(int handle,LPRECT cur,LPCRECT org) {
	CRect rc(cur);

	int w = rc.Width();
	int h = rc.Height();
	int nSize = max(abs(w),abs(h));
	w = SignAbs(w,nSize);
	h = SignAbs(h,nSize);

	rc.right = rc.left + w;
	rc.bottom = rc.top + h;

	*cur = rc;
}

CreateRectTracker::CreateRectTracker() 
:	ShiftRectTracker(&AdjustRectCreate,0){
}

BOOL CreateRectTracker::TrackCreate(CWnd* pWnd,CPoint point) {
	return ShiftRectTracker::TrackRubberBand(pWnd,point,TRUE);
}

CreateCircleTracker::CreateCircleTracker() 
:	ShiftRectTracker(AdjustRectCreate,0) {
}

BOOL CreateCircleTracker::TrackCreate(CWnd* pWnd,CPoint point){
	return ShiftRectTracker::TrackRubberBand(pWnd,point,TRUE);
}

void CreateCircleTracker::DrawTrackerRect(LPCRECT lpRect,CWnd* pWndClipTo,CDC* pDC,CWnd* pWnd) {
	ASSERT_VALID(pDC);
	VERIFY(pDC->SaveDC() != 0);

	CPen pen;
	VERIFY(pen.CreatePen(PS_SOLID,1,RGB(0x00,0x00,0x00)));
	CBrush brush;
	VERIFY(brush.CreateStockObject(NULL_BRUSH));
	CPen* pPenOld     = pDC->SelectObject(&pen);
	CBrush* pBrushOld = pDC->SelectObject(&brush);

	CRect rc(lpRect);
	rc.NormalizeRect();
	pDC->SetROP2(R2_NOT);
	pDC->Ellipse(rc);

	pDC->SelectObject(pPenOld);
	pDC->SelectObject(pBrushOld);

	VERIFY(pDC->RestoreDC(-1));
}

MoveRgnTracker::MoveRgnTracker() {
}

CPoint MoveRgnTracker::GetMoveOffset() const {
	return m_szOffset;
}

BOOL MoveRgnTracker::TrackMove(CRgn& rgn,CWnd* pWnd ,CPoint devOrg) {
	ASSERT_VALID(pWnd);
	if (!rgn.PtInRegion(devOrg))
		return FALSE;

	CClientDC dc(pWnd);

	CPoint ptCur = devOrg;

	AfxLockTempMaps();
	DrawTrackerRgn(&rgn,&dc);

	pWnd->SetCapture();
	BOOL bLoop = TRUE;
	while(bLoop) {
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			bLoop = FALSE;
			break;
		case WM_MOUSEMOVE:{
			CPoint ptMouse;
			ptMouse.x = (int)(short)LOWORD(msg.lParam);
			ptMouse.y = (int)(short)HIWORD(msg.lParam);
			if (ptCur != ptMouse) {
				DrawTrackerRgn(&rgn,&dc);
				if (::IsShifted())
					AdjustMove(ptMouse,devOrg);
				CPoint ptOffset = ptMouse - ptCur;
				rgn.OffsetRgn(ptOffset);
				DrawTrackerRgn(&rgn,&dc);
				ptCur = ptMouse;
			}
		}
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}
	VERIFY(ReleaseCapture());
	DrawTrackerRgn(&rgn,&dc);
	m_szOffset = ptCur - devOrg;
	AfxUnlockTempMaps(FALSE);
	return m_szOffset.cx != 0 || m_szOffset.cy != 0;
}

void MoveRgnTracker::DrawTrackerRgn(CRgn* prgnDev,CDC* pDC) const {
	ASSERT_VALID(pDC);
	int nSaved = pDC->SetROP2(R2_NOT);
	pDC->FrameRgn(prgnDev,&CBrush(RGB(0,0,0)),1,1);
	pDC->SetROP2(nSaved);
}


RotateTracker::RotateTracker() {
	
}

void RotateTracker::SetTracker(LPCRECT devRect,CPoint devCenter,double rotate) {
	m_devCenter = devCenter;
	m_rotate    = rotate;
	m_rc        = devRect;
}

BOOL RotateTracker::TrackRotate(CWnd* pWnd,CPoint ptMouse) {
	int hit = HitTest(ptMouse);
	if (hit == CRectTracker::hitNothing) {
		return FALSE;
	}
	if (hit == RotateTracker::hitRotateCenter) {
		return TrackCenter(pWnd,ptMouse);
	}
	return TrackHandle(pWnd,ptMouse);
}

BOOL RotateTracker::TrackHandle(CWnd* pWnd,CPoint ptMouse) {
	ASSERT_VALID(pWnd);
	RotateTrackParam param;
	param.m_rotatebase  = m_rotate;
	param.m_rotatestart = CalcRotate(ptMouse,m_devCenter);
	TTrackerMsgPump<RotateTracker,RotateTrackParam> msgpump(*this,param);
	return msgpump.Track(pWnd);		
}

BOOL RotateTracker::TrackCenter(CWnd* pWnd,CPoint ptMouse) {
	ASSERT_VALID(pWnd);
	CenterTrackParam param;
	TTrackerMsgPump<RotateTracker,CenterTrackParam> msgpump(*this,param);
	return msgpump.Track(pWnd);	
}

void RotateTracker::OnMouseMove(TrackerMsgPump* p,CenterTrackParam& param) {
	CPoint ptWnd = ConvertLParam2Point(p->m_msg.lParam);
	
	DrawTracking(p->m_pDC);
	m_devCenter = ptWnd;
	DrawTracking(p->m_pDC);
}

void RotateTracker::OnMouseMove(TrackerMsgPump* p,RotateTrackParam& param) {
	CPoint ptWnd = ConvertLParam2Point(p->m_msg.lParam);
	
	DrawTracking(p->m_pDC);
	m_rotate = param.m_rotatebase + CalcRotate(ptWnd,m_devCenter) - param.m_rotatestart;
	if (::IsShifted()) {
		m_rotate = AdjustRotate(m_rotate);
	}
	DrawTracking(p->m_pDC);
}

void RotateTracker::DrawTracking(CDC* pDC) {
	ASSERT_VALID(pDC);
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetROP2(R2_NOT);

	DrawBorder(pDC);
	DrawCenter(pDC);

	VERIFY(pDC->RestoreDC(-1));
}

int RotateTracker::HitTest(CPoint pt) const {
	if (HitPoint(m_devCenter,pt)) {
		return RotateTracker::hitRotateCenter;
	}
	for (int n = 0;n < 8;n++) {
		if (HitPoint(GetHandlePos(n),pt))
			return n;
	}
	return CRectTracker::hitNothing;
}

BOOL RotateTracker::HitPoint(POINT ptTarget,POINT ptMouse) const {
	CRect rc;
	SetRectByCenter(rc,ptTarget,DEFAULT_TRACKER_HANDLE_SIZE);
	return rc.PtInRect(ptMouse);
}

void RotateTracker::Draw(CDC* pDC) const {
	ASSERT_VALID(pDC);
	VERIFY(pDC->SaveDC() != 0);
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

	DrawBorder(pDC);	
	DrawCenter(pDC);
	DrawHandle(pDC);

	VERIFY(pDC->RestoreDC(-1));
}

void RotateTracker::GetTracker(CPoint& devCenter,double& rotate) const {
	devCenter = m_devCenter;
	rotate    = m_rotate;
}

void RotateTracker::DrawBorder(CDC* pDC) const {
	ASSERT_VALID(pDC);

	CPen pen(PS_DOT,1,RGB(0x00,0x00,0x00));
	CPen* pPenOld = pDC->SelectObject(&pen);

	POINT pt[5];
	GetTrackerPolyline(pt);
	VERIFY(pDC->Polyline(pt,5));

	pDC->SelectObject(pPenOld);
}

void RotateTracker::DrawHandle(CDC* pDC) const {
	ASSERT_VALID(pDC);
	
	CPen pen(PS_SOLID,1,RGB(0x00,0x00,0x00));
	CBrush brush(RGB(0xFF,0xFF,0xFF));

	CPen* pPenOld     = pDC->SelectObject(&pen);
	CBrush* pBrushOld = pDC->SelectObject(&brush);
	for (int n = 0;n < 8;n ++) {
		CRect rc;
		SetRectByCenter(rc,GetHandlePos(n),DEFAULT_TRACKER_HANDLE_SIZE);
		VERIFY(pDC->Ellipse(rc));
	}

	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);
}

void RotateTracker::DrawCenter(CDC* pDC) const {
	ASSERT_VALID(pDC);

	CPen pen(PS_SOLID,2,RGB(0xFF,0xFF,0xFF));
	CBrush brush(RGB(0x00,0x00,0x00));

	CPen* pPenOld     = pDC->SelectObject(&pen);
	CBrush* pBrushOld = pDC->SelectObject(&brush);

	CRect rc;
	SetRectByCenter(rc,m_devCenter,DEFAULT_TRACKER_CENTER_SIZE);
	VERIFY(pDC->Ellipse(rc));

	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);
}

CPoint RotateTracker::GetHandlePos(int nHandle) const {
	CPoint pt = ::GetHandlePos(m_rc,nHandle);
	RotatePoint(pt,m_devCenter,m_rotate);
	return pt;
}

void RotateTracker::GetTrackerPolyline(LPPOINT ppt) const {
	ASSERT(AfxIsValidAddress(ppt,sizeof POINT * 5));
	SetPoint(&ppt[0],m_rc.left, m_rc.top);
	SetPoint(&ppt[1],m_rc.right,m_rc.top);
	SetPoint(&ppt[2],m_rc.right,m_rc.bottom);
	SetPoint(&ppt[3],m_rc.left, m_rc.bottom);
	SetPoint(&ppt[4],m_rc.left, m_rc.top);
	for (int n = 0; n < 5;n++ ) {
		POINT& pt = ppt[n];
		RotatePoint(pt,m_devCenter,m_rotate);
	}
}

TrackerEx::TrackerEx() {
}

void TrackerEx::SetTracker(LPCRECT prc,CPoint pt,double scalex,double scaley,double rotate) {
	ASSERT(AfxIsValidAddress(prc,sizeof RECT,FALSE));
	m_rc = prc;
	m_center = pt;
	m_scalex = scalex;
	m_scaley = scaley;
	m_rotate = rotate;
}

void TrackerEx::GetTracker(double& scalex,double& scaley,double& rotate) const {
	ASSERT(AfxIsValidAddress(&scalex,sizeof(double)));
	ASSERT(AfxIsValidAddress(&scaley,sizeof(double)));
	ASSERT(AfxIsValidAddress(&rotate,sizeof(double)));
	scalex  = m_scalex;
	scaley  = m_scaley;
	rotate = m_rotate;
}

int TrackerEx::HitTest(CPoint ptWnd) const {
	for (int n = 0;n < 8;n++) {
		if (HitPoint(GetHandlePos(n),ptWnd))
			return n;
	}
	return CRectTracker::hitNothing;
}

BOOL TrackerEx::HitPoint(POINT ptTarget,POINT ptMouse) const {
	CRect rc;
	SetRectByCenter(rc,ptTarget,DEFAULT_TRACKER_HANDLE_SIZE);
	return rc.PtInRect(ptMouse);
}

void TrackerEx::Draw(CDC* pDC) const {
	ASSERT_VALID(pDC);
	int n = pDC->SaveDC();
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0,0);
	pDC->SetWindowOrg(0,0);
	
	DrawBorder(pDC);
	DrawHandle(pDC);

	pDC->RestoreDC(n);
}

void TrackerEx::DrawHandle(CDC* pDC) const {
	ASSERT_VALID(pDC);
	
	CPen pen(PS_SOLID,1,RGB(0x00,0x00,0x00));
	CBrush brush(RGB(0xFF,0xFF,0xFF));

	CPen* pPenOld     = pDC->SelectObject(&pen);
	CBrush* pBrushOld = pDC->SelectObject(&brush);

	for (int n = 0;n < 8;n ++) {
		CRect rc;
		SetRectByCenter(rc,GetHandlePos(n),DEFAULT_TRACKER_HANDLE_SIZE);
		DrawHandle1(pDC,rc);
	}

	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);
}

void TrackerEx::DrawTracking(CDC* pDC) {
	int nSaved = pDC->SaveDC();
	pDC->SetROP2(R2_NOT);
	DrawBorder(pDC);
	pDC->RestoreDC(nSaved);
}

void TrackerEx::DrawBorder(CDC* pDC) const {
	ASSERT_VALID(pDC);

	CPen pen(PS_DOT,1,RGB(0x00,0x00,0x00));
	CPen* pPenOld = pDC->SelectObject(&pen);

	POINT apt[5];
	GetTrackerPolyline(apt);
	VERIFY(pDC->Polyline(apt,5));

	pDC->SelectObject(pPenOld);
}

void TrackerEx::DrawCenter(CDC* pDC) const {
	ASSERT_VALID(pDC);
	
	CPen pen(PS_SOLID,2,RGB(0xFF,0xFF,0xFF));
	CBrush brush(RGB(0x00,0x00,0x00));

	CPen* pPenOld     = pDC->SelectObject(&pen);
	CBrush* pBrushOld = pDC->SelectObject(&brush);

	CRect rc;
	SetRectByCenter(rc,m_center,DEFAULT_TRACKER_CENTER_SIZE);
	VERIFY(pDC->Ellipse(rc));

	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);
}

CPoint TrackerEx::GetHandlePos(int nHandle) const {
	CPoint pt = ::GetHandlePos(m_rc,nHandle);
	ScalePoint (pt,m_center,m_scalex,m_scaley);
	RotatePoint(pt,m_center,m_rotate);
	return pt;
}

void TrackerEx::GetTrackerPolyline(LPPOINT ppt) const {
	ASSERT(AfxIsValidAddress(ppt,sizeof POINT * 5));
	SetPoint(&ppt[0],m_rc.left, m_rc.top);
	SetPoint(&ppt[1],m_rc.right,m_rc.top);
	SetPoint(&ppt[2],m_rc.right,m_rc.bottom);
	SetPoint(&ppt[3],m_rc.left, m_rc.bottom);
	SetPoint(&ppt[4],m_rc.left, m_rc.top);
	for (int n = 0; n < 5;n++ ) {
		POINT& pt = ppt[n];
		ScalePoint (pt,m_center,m_scalex,m_scaley);
		RotatePoint(pt,m_center,m_rotate);
	}
}


void RotateTrackerEx::DrawHandle1(CDC* pDC,LPCRECT prc) const {
	VERIFY(pDC->Ellipse(prc));
}

BOOL RotateTrackerEx::TrackRotate(CWnd* pWnd,CPoint ptWnd) {
	ASSERT_VALID(pWnd);
	int nHandle = HitTest(ptWnd);
	if (nHandle == CRectTracker::hitNothing ||
		nHandle == CRectTracker::hitMiddle)
		return FALSE;
	ASSERT(0 <= nHandle && nHandle < 8);

	RotateTrackParam param;
	param.m_rotatebase  = m_rotate;
	param.m_rotatestart = CalcRotate(ptWnd,m_center);
	TTrackerMsgPump<RotateTrackerEx,RotateTrackParam> msgpump(*this,param);
	return msgpump.Track(pWnd);
}

void RotateTrackerEx::OnMouseMove(TrackerMsgPump* p,RotateTrackParam& param) {
	DrawTracking(p->m_pDC);

	CPoint ptWnd = ConvertLParam2Point(p->m_msg.lParam);
	m_rotate = param.m_rotatebase + CalcRotate(ptWnd,m_center)- param.m_rotatestart;
	if (IsShifted()) {
		m_rotate = AdjustRotate(m_rotate);
	}
	DrawTracking(p->m_pDC);
}


void ResizeTrackerEx::DrawHandle1(CDC* pDC,LPCRECT prc) const {
	VERIFY(pDC->Rectangle(prc));
}

BOOL ResizeTrackerEx::TrackResize(CWnd* pWnd,CPoint ptWnd) {
	ASSERT_VALID(pWnd);
	const int nHandle = HitTest(ptWnd);
	if (nHandle < 0)
		return FALSE;
	ASSERT(0 <= nHandle && nHandle < 8);

	ResizeTrackParam param;
	CSize  sz  = ptWnd - m_center;
	param.m_scalingold = distance(sz.cx,sz.cy);
	param.m_scalexold  = m_scalex;
	param.m_scaleyold  = m_scaley;
	param.m_nHandle  = nHandle;
	TTrackerMsgPump<ResizeTrackerEx,ResizeTrackParam> msgpump(*this,param);
	return msgpump.Track(pWnd);
}

void ResizeTrackerEx::OnMouseMove(TrackerMsgPump* p,ResizeTrackParam& param) {
	DrawTracking(p->m_pDC);

	const CPoint ptWnd  = ConvertLParam2Point(p->m_msg.lParam);
	const CSize  sz  = ptWnd - m_center;
	const double scalenew  = distance(sz.cx,sz.cy);
	const double scaleold  = param.m_scalingold;
	const int nHandle = param.m_nHandle;
	m_scalex = param.m_scalexold;
	m_scaley = param.m_scaleyold;

	switch(nHandle) {
	case CRectTracker::hitLeft:
	case CRectTracker::hitTopLeft:
	case CRectTracker::hitBottomLeft:
	case CRectTracker::hitRight:
	case CRectTracker::hitTopRight:
	case CRectTracker::hitBottomRight:
		m_scalex = m_scalex * scalenew  / scaleold;
		break;
	case CRectTracker::hitTop:
	case CRectTracker::hitBottom:
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	switch(nHandle) {
	case CRectTracker::hitTop:
	case CRectTracker::hitBottom:
	case CRectTracker::hitTopLeft:
	case CRectTracker::hitBottomLeft:
	case CRectTracker::hitTopRight:
	case CRectTracker::hitBottomRight:
		m_scaley = m_scaley * scalenew  / scaleold;
		break;
	case CRectTracker::hitLeft:
	case CRectTracker::hitRight:
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	DrawTracking(p->m_pDC);
}

BOOL ResizeTrackerEx::SetCursor(CWnd* pWnd,UINT nHitTest) const {
	if (nHitTest != HTCLIENT) {
		return FALSE;
	}
	CPoint pt;
	if (!GetCursorPos(&pt)) {
		ASSERT(FALSE);
		return FALSE;
	}
	ASSERT_VALID(pWnd);
	pWnd->ScreenToClient(&pt);

	int nHit = HitTest(pt);
	if (nHit < 0) {
		return FALSE;
	}

	const int nType = CalcRectHandleType(pt,m_center);
	LPCTSTR ptszCursor = RectHandleType2Cursor(nType);
	ASSERT(ptszCursor != NULL);
	HCURSOR hcur = ::LoadCursor(NULL,ptszCursor);
	ASSERT(hcur != NULL);
	::SetCursor(hcur);
	return TRUE;
}

void InsPathPtTracker::GetTracker(CPoint& pt,CPoint& ptCurveL,CPoint& ptCurveR) const {
	pt = m_pt;
	ptCurveL = m_ptCurveL;
	ptCurveR = m_ptCurveR;
}

void InsPathPtTracker::OnMouseMove(TrackerMsgPump* p,InsPathPtTrackerParam& param) {
	CPoint pt = ConvertLParam2Point(p->m_msg.lParam);
	DrawTracking(p->m_pDC);
	m_ptCurveL = pt;
	m_ptCurveR = m_pt + (m_pt - m_ptCurveL);
	DrawTracking(p->m_pDC);
}

void InsPathPtTracker::DrawTracking(CDC* pDC) {
	int nSaved = pDC->SetROP2(R2_NOT);

	if (m_instype == insNext || m_instype == insIns) {
		POINT apt[7];
		apt[0] = m_ptPrev;
		apt[1] = m_ptPrevCurve;
		apt[2] = m_ptCurveL;
		apt[3] = m_pt;

		int nPoints;
		if (m_instype == insIns) {
			apt[4] = m_ptCurveR;
			apt[5] = m_ptNextCurve;
			apt[6] = m_ptNext;
			nPoints = 7;
		} else {
			ASSERT(m_instype == insNext);
			nPoints = 4;
		}
		VERIFY(pDC->PolyBezier(apt,nPoints));
	}

	if (m_pt != m_ptCurveR) {
		CRect rcHandle1;
		CRect rcHandle2;
		SetRectByCenter(rcHandle1,m_ptCurveL,DEFAULT_TRACKER_HANDLE_SIZE);
		SetRectByCenter(rcHandle2,m_ptCurveR,DEFAULT_TRACKER_HANDLE_SIZE);
		
		pDC->MoveTo(m_ptCurveL);
		pDC->LineTo(m_pt);
		pDC->LineTo(m_ptCurveR);

		pDC->Ellipse(rcHandle1);
		pDC->Ellipse(rcHandle2);
	}

	CRect rc;
	SetRectByCenter(rc,m_pt,DEFAULT_TRACKER_HANDLE_SIZE);
	pDC->Rectangle(rc);

	pDC->SetROP2(nSaved);
}

BOOL InsPathPtTracker::TrackAddPointFirst(CWnd* pWnd,CPoint pt) {
	m_instype = insFirst;
	m_pt = pt;
	return _TrackAddPoint(pWnd,pt);
}

BOOL InsPathPtTracker::TrackAddPointNext(CWnd* pWnd,CPoint pt,CPoint pt1,CPoint pt2) {
	m_instype     = insNext;
	m_pt          = pt;
	m_ptPrev      = pt1;
	m_ptPrevCurve = pt2;
	return _TrackAddPoint(pWnd,pt);
}

BOOL InsPathPtTracker::TrackAddPointIns(CWnd* pWnd,CPoint pt,CPoint pt1,CPoint pt2,CPoint pt3,CPoint pt4) {
	m_instype     = insIns;
	m_pt          = pt;
	m_ptPrev      = pt1;
	m_ptPrevCurve = pt2;
	m_ptNextCurve = pt3;
	m_ptNext      = pt4;
	return _TrackAddPoint(pWnd,pt);
}

BOOL InsPathPtTracker::_TrackAddPoint(CWnd* pWnd,CPoint pt) {
	m_ptCurveL    = m_pt;
	m_ptCurveR    = m_pt;
	InsPathPtTrackerParam param;	
	TTrackerMsgPump<InsPathPtTracker,InsPathPtTrackerParam> tracker(*this,param);
	return tracker.Track(pWnd);
}

ModPathPtTracker::ModPathPtTracker() 
:	m_bHasNext(FALSE),
	m_bHasPrev(FALSE) {
}

void ModPathPtTracker::SetPoint(PointType type,CPoint pt,CPoint ptL,CPoint ptR) {
	m_pt = pt;
	m_ptCurveL = ptL;
	m_ptCurveR = ptR;
	m_pointType = type;
}

void ModPathPtTracker::SetPrevPoint(CPoint ptPrev,CPoint ptPrevCurve) {
	ASSERT(!m_bHasPrev);
	m_bHasPrev = TRUE;
	m_ptPrev = ptPrev;
	m_ptPrevCurve = ptPrevCurve;
}

void ModPathPtTracker::SetNextPoint(CPoint ptNext,CPoint ptNextCurve) {
	ASSERT(!m_bHasNext);
	m_bHasNext = TRUE;
	m_ptNext = ptNext;
	m_ptNextCurve = ptNextCurve;
}

BOOL ModPathPtTracker::TrackHandleL(CWnd* pWnd,CPoint pt) {
	return Track(handleLeft,pWnd,pt);
}

BOOL ModPathPtTracker::TrackHandleR(CWnd* pWnd,CPoint pt) {
	return Track(handleRight,pWnd,pt);
}

void ModPathPtTracker::GetTracker(int& n,CPoint& pt,CPoint& ptL,CPoint& ptR) const {
	n = m_pointType;
	pt = m_pt;
	ptL = m_ptCurveL;
	ptR = m_ptCurveR;
}

BOOL ModPathPtTracker::Track(HandleType handle,CWnd* pWnd,CPoint pt) {
	m_handle = handle;

	ModPathPtTrackerParam param;
	TTrackerMsgPump<ModPathPtTracker,ModPathPtTrackerParam> tracker(*this,param);
	return tracker.Track(pWnd);
}

void ModPathPtTracker::DrawTracking(CDC* pDC) {
	int nSavedROP = pDC->SetROP2(R2_NOT);

	if (m_bHasPrev) {
		POINT apt[4];
		apt[0] = m_ptPrev;
		apt[1] = m_ptPrevCurve;
		apt[2] = m_ptCurveL;
		apt[3] = m_pt;
		pDC->PolyBezier(apt,4);
	}

	if (m_bHasNext) {
		POINT apt[4];
		apt[0] = m_pt;
		apt[1] = m_ptCurveR;
		apt[2] = m_ptNextCurve;
		apt[3] = m_ptNext;
		pDC->PolyBezier(apt,4);
	}

	//	方向線描画
	pDC->MoveTo(m_ptCurveL);
	pDC->LineTo(m_pt);
	pDC->LineTo(m_ptCurveR);

	//	ポイント描画
	CRect rc;
	SetRectByCenter(rc,m_pt,DEFAULT_TRACKER_HANDLE_SIZE);
	VERIFY(pDC->Rectangle(rc));

	//	ハンドル描画
	CRect rcHandle1;
	SetRectByCenter(rcHandle1,m_ptCurveL,DEFAULT_TRACKER_HANDLE_SIZE);
	pDC->Ellipse(rcHandle1);

	CRect rcHandle2;
	SetRectByCenter(rcHandle2,m_ptCurveR,DEFAULT_TRACKER_HANDLE_SIZE);
	pDC->Ellipse(rcHandle2);

	pDC->SetROP2(nSavedROP);
}

inline void SetSlavePtByMasterPt(CPoint ptNewMaster,CPoint ptOrg,BOOL bSmooth,CPoint& ptMaster,CPoint& ptSlave) {
	ptMaster = ptNewMaster;
	if (!bSmooth)
		return;

	CSize vectorMaster = ptMaster - ptOrg;
	double distanceSlave  = distance(ptSlave - ptOrg);
	double distanceMaster = distance(vectorMaster);
	CSize vectorNewSlave = (distanceSlave / distanceMaster) * (-vectorMaster);
	if (1 <= distance(vectorNewSlave)) {
		ptSlave = ptOrg + vectorNewSlave;
	}
}

void ModPathPtTracker::OnMouseMove(TrackerMsgPump* p,ModPathPtTrackerParam& param) {
	DrawTracking(p->m_pDC);

	BOOL bAlt = ::IsStillKeyDown(VK_MENU);
	BOOL bSmooth = (m_pointType != pointCurve) ? !bAlt : bAlt;

	CPoint ptCur = ConvertLParam2Point(p->m_msg.lParam);
	if (m_handle == handleLeft) {
		SetSlavePtByMasterPt(ptCur,m_pt,bSmooth,m_ptCurveL,m_ptCurveR);
	} else {
		ASSERT(handleRight == m_handle);
		SetSlavePtByMasterPt(ptCur,m_pt,bSmooth,m_ptCurveR,m_ptCurveL);
	}

	DrawTracking(p->m_pDC);
}



MovPathPtTracker::MovPathPtTracker() {
}

void MovPathPtTracker::AppendBeizer(PCBPOINT pcbpt,UINT nLen) {
	ASSERT(0 < nLen);
	ASSERT(AfxIsValidAddress(pcbpt,sizeof BPOINT * nLen,FALSE));
	BEIZER beizer = {pcbpt,nLen};
	m_beizers.push_back(beizer);
}

void MovPathPtTracker::ClearAllBeizers() {
	m_beizers.clear();
}

BOOL MovPathPtTracker::TrackMove(CWnd* pWnd,CPoint pt) {
	ASSERT(m_beizers.size() != 0);
	MovPathPtTrackerParam param;	
	param.m_ptOld   = pt;
	param.m_ptStart = pt;
	m_ptMoved = CPoint(0,0);
	TTrackerMsgPump<MovPathPtTracker,MovPathPtTrackerParam> tracker(*this,param);
	BOOL b = tracker.Track(pWnd);
	ClearAllBeizers();
	return b;
}

void MovPathPtTracker::OnMouseMove(TrackerMsgPump* p,MovPathPtTrackerParam& param) {
	CPoint ptCur(ConvertLParam2Point(p->m_msg.lParam));
	CPoint ptOffset = ptCur - param.m_ptOld;
	
	DrawTracking(p->m_pDC);
	m_ptMoved = ptCur - param.m_ptStart;
	DrawTracking(p->m_pDC);
}

CPoint MovPathPtTracker::GetTracked() const {
	ASSERT(m_beizers.size() == 0);
	return m_ptMoved;
}

void MovPathPtTracker::DrawTracking(CDC* pDC) {
	int nSaved = pDC->SaveDC();
	pDC->SetROP2(R2_NOT);
	DrawBeizers(pDC);
	VERIFY(pDC->RestoreDC(nSaved));
}

void MovPathPtTracker::DrawBeizers(CDC* pDC) const {
	BEIZERS::const_iterator i;
	for (i = m_beizers.begin();i != m_beizers.end();i++) {
		DrawBeizer(pDC,*i);
	}
}

void MovPathPtTracker::DrawBeizer(CDC* pDC,const BEIZER& beizer) const {
	DrawBeizerLine(pDC,beizer);
	DrawBeizerPts(pDC,beizer);
}

void MovPathPtTracker::DrawBeizerPts(CDC* pDC,const BEIZER& beizer) const {
	PCBPOINT p = beizer.m_pPts;
	int nBLen = beizer.m_nLen;
	while (0 < nBLen--) {
		ASSERT(AfxIsValidAddress(p,sizeof BPOINT));
		if (!p->m_bSel) {
			p++;
			continue;
		}
		BPOINT bp;
		GetMovedBPOINT(bp,p++);
		CPoint pt = bp.m_pt;
		CRect rc;
		SetRectByCenter(rc,bp.m_pt,DEFAULT_TRACKER_HANDLE_SIZE);
		CRect rcL;
		SetRectByCenter(rcL,bp.m_ptHandleL,DEFAULT_TRACKER_HANDLE_SIZE);
		CRect rcR;
		SetRectByCenter(rcR,bp.m_ptHandleR,DEFAULT_TRACKER_HANDLE_SIZE);
		BOOL bHasL = !(pt == bp.m_ptHandleL);
		BOOL bHasR = !(pt == bp.m_ptHandleR);

		pDC->Rectangle(rc);
		if (bHasL) {
			pDC->Ellipse(rcL);
		}
		if (bHasR) {
			pDC->Ellipse(rcR);
		}
		if (bHasL) {
			pDC->MoveTo(bp.m_pt);
			pDC->LineTo(bp.m_ptHandleL);
		}
		if (bHasR) {
			pDC->MoveTo(bp.m_pt);
			pDC->LineTo(bp.m_ptHandleR);
		}	
	}	
}

void MovPathPtTracker::DrawBeizerLine(CDC* pDC,const BEIZER& beizer) const {
	CArray<POINT,POINT> pts;
	const int numPts = (beizer.m_nLen) * 3  + 1;
	pts.SetSize(numPts);
	int n = 0;
	BPOINT bp;
	PCBPOINT p = beizer.m_pPts;
	GetMovedBPOINT(bp,p++);
	pts.SetAt(n++,bp.m_pt);
	pts.SetAt(n++,bp.m_ptHandleR);

	int nBLen = beizer.m_nLen -1;
	while (0 < nBLen--) {
		ASSERT(AfxIsValidAddress(p,sizeof BPOINT));
		GetMovedBPOINT(bp,p++);
		pts.SetAt(n++,bp.m_ptHandleL);
		pts.SetAt(n++,bp.m_pt);
		pts.SetAt(n++,bp.m_ptHandleR);
	}
	GetMovedBPOINT(bp,beizer.m_pPts);
	pts.SetAt(n++,bp.m_ptHandleL);
	pts.SetAt(n++,bp.m_pt);
	ASSERT(n == numPts);
	pDC->PolyBezier(pts.GetData(),numPts);
}

void MovPathPtTracker::GetMovedBPOINT(BPOINT& bp,PCBPOINT p) const {
	ASSERT(AfxIsValidAddress(p,sizeof BPOINT,FALSE));
	ASSERT(AfxIsValidAddress(&bp,sizeof BPOINT));
	bp = *p;
	if (bp.m_bSel) {
		bp.m_pt = m_ptMoved + bp.m_pt;
		bp.m_ptHandleL = m_ptMoved + bp.m_ptHandleL;
		bp.m_ptHandleR = m_ptMoved + bp.m_ptHandleR;
	}
}





MoveBezierTracker::MoveBezierTracker() :
	m_pBezier(NULL),
	m_szOffset(0,0){
}

CPoint MoveBezierTracker::GetMoveOffset() const {
	return m_szOffset;
}

void MoveBezierTracker::SetTracker(CBEZIER* p) {
	ASSERT(m_pBezier == NULL);
	m_pBezier = p;
	ASSERT(m_pBezier != NULL);
}

BOOL MoveBezierTracker::TrackMove(CWnd* pWnd ,CPoint devOrg) {
	ASSERT_VALID(pWnd);
	if (!PtInBezier(*m_pBezier,devOrg))
		return FALSE;

	MoveBezierTrackerParam param;
	param.m_ptOrg = devOrg;
	TTrackerMsgPump<MoveBezierTracker,MoveBezierTrackerParam> pumper(*this,param);
	pumper.Track(pWnd);
	return m_szOffset.cx != 0 || m_szOffset.cy != 0;
}

void MoveBezierTracker::DrawTracking(CDC* pDC) {
	ASSERT_VALID(pDC);
	int nSaved = pDC->SetROP2(R2_NOT);
	pDC->SetViewportOrg(CPoint(m_szOffset));
	PolyBezier(pDC,*m_pBezier);
	pDC->SetViewportOrg(CPoint(0,0));
	pDC->SetROP2(nSaved);
}

void MoveBezierTracker::OnMouseMove(TrackerMsgPump* p,MoveBezierTrackerParam& param) {
	CPoint ptMouse = ConvertLParam2Point(p->m_msg.lParam);
	DrawTracking(p->m_pDC);

	if (::IsShifted())
		AdjustMove(ptMouse,param.m_ptOrg);
	m_szOffset = ptMouse - param.m_ptOrg;
	DrawTracking(p->m_pDC);
}
