#include "stdafx.h"
#include "InWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(InWnd, CWnd)
	//{{AFX_MSG_MAP(InWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
	ウインドウ内の描画&操作用子ウインドウ
*/
InWnd::InWnd()
:	m_pOwner(NULL)
{
}


InWnd::~InWnd()
{
}

void InWnd::OnPaint() 
{
	if (m_pOwner != NULL)
		m_pOwner->OnPaint(this);
	else
	{
		ASSERT(FALSE);
		CPaintDC dc(this);
	}
}


void InWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_pOwner != NULL)
		m_pOwner->OnLButtonDblClk(this,nFlags,point);
	else
		CWnd::OnLButtonDblClk(nFlags, point);
}

void InWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetActiveWindow();
	SetFocus();
	if (m_pOwner != NULL)
		m_pOwner->OnLButtonDown(this,nFlags,point);
	else
		CWnd::OnLButtonDown(nFlags, point);
}

void InWnd::OnLButtonUp(UINT nFlags, CPoint point) {
	if (m_pOwner != NULL)
		m_pOwner->OnLButtonUp(this,nFlags,point);
	else
		CWnd::OnLButtonUp(nFlags, point);
}

void InWnd::OnRButtonDblClk(UINT nFlags, CPoint point) {
	if (m_pOwner != NULL)
		m_pOwner->OnRButtonDblClk(this,nFlags,point);
	else
		CWnd::OnRButtonDblClk(nFlags, point);
}

void InWnd::OnRButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	if (m_pOwner != NULL)
		m_pOwner->OnRButtonDown(this,nFlags,point);
	else
		CWnd::OnRButtonDown(nFlags, point);
}

void InWnd::OnRButtonUp(UINT nFlags, CPoint point) {
	if (m_pOwner != NULL)
		m_pOwner->OnRButtonUp(this,nFlags,point);
	else
		CWnd::OnRButtonUp(nFlags, point);
}

BOOL InWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	if (m_pOwner != NULL)
		return m_pOwner->OnSetCursor(this,pWnd,nHitTest,message);
	else
		return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL InWnd::Create(InWndOwner* p,CWnd* pParentWnd,LPCRECT prc,UINT nID,DWORD dwStyle,DWORD dwExStyle)
{
	ASSERT(p != NULL);
	m_pOwner = p;

	ASSERT_VALID(pParentWnd);

	LPCTSTR ptszClassName = ::AfxRegisterWndClass(CS_OWNDC,::LoadCursor(NULL,IDC_ARROW),NULL,NULL);
	ASSERT(AfxIsValidString(ptszClassName));

	CRect rc;
	if (prc == NULL)
		rc.SetRectEmpty();
	else
		rc = *prc;

	return CWnd::CreateEx(dwExStyle,ptszClassName,NULL,dwStyle,rc,pParentWnd,nID,NULL);
}

BOOL InWnd::OnEraseBkgnd(CDC*) 
{
	return TRUE;	
}


/**
	InWndの描画イベントキャッチインターフェース
*/
InWndOwner::InWndOwner()
{
}

InWndOwner::~InWndOwner()
{
}


void InWndOwner::OnPaint(InWnd* p)
{
	ASSERT_VALID(p);
	CPaintDC dc(p);
	ASSERT(FALSE);
}

void InWndOwner::OnLButtonDblClk(InWnd* p,UINT nFlags, CPoint point)
{
	ASSERT_VALID(p);
	p->CWnd::OnLButtonDblClk(nFlags,point);
}

void InWndOwner::OnLButtonDown(InWnd* p,UINT nFlags, CPoint point)
{
	ASSERT_VALID(p);
	p->CWnd::OnLButtonDown(nFlags,point);
}

void InWndOwner::OnLButtonUp(InWnd* p,UINT nFlags, CPoint point)
{
	ASSERT_VALID(p);
	p->CWnd::OnLButtonUp(nFlags,point);
}

void InWndOwner::OnRButtonDblClk(InWnd* p,UINT nFlags, CPoint point)
{
	ASSERT_VALID(p);
	p->CWnd::OnRButtonDblClk(nFlags,point);
}

void InWndOwner::OnRButtonDown(InWnd* p,UINT nFlags, CPoint point)
{
	ASSERT_VALID(p);
	p->CWnd::OnRButtonDown(nFlags,point);
}

void InWndOwner::OnRButtonUp(InWnd* p,UINT nFlags, CPoint point)
{
	ASSERT_VALID(p);
	p->CWnd::OnRButtonUp(nFlags,point);
}

BOOL InWndOwner::OnSetCursor(InWnd* p,CWnd* pWnd, UINT nHitTest, UINT message)
{
	ASSERT_VALID(p);
	return p->CWnd::OnSetCursor(pWnd,nHitTest,message);
}

UINT InWnd::OnGetDlgCode() {	
	return CWnd::OnGetDlgCode();
	//return DLGC_BUTTON;
	//return DLGC_BUTTON | DLGC_UNDEFPUSHBUTTON | DLGC_WANTALLKEYS;
}
