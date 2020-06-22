#include "stdafx.h"
#include "PreviewStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void PreviewStatic::PaintEventListener::OnPSPaint(PreviewStatic& ps) {
	(&ps)->CStatic::OnPaint();
}

BOOL PreviewStatic::PaintEventListener::OnPSEraseBkgnd(PreviewStatic& ps,CDC& dc) {
	return (&ps)->CStatic::OnEraseBkgnd(&dc);
}

void PreviewStatic::PaintEventListener::OnPSNcPaint(PreviewStatic& ps) {
	(&ps)->CStatic::OnNcPaint();
}

void PreviewStatic::PaintEventListener::OnPSMouseMove(PreviewStatic& ps,UINT nFlags, CPoint point) {
	(&ps)->CStatic::OnMouseMove(nFlags,point);
}

PreviewStatic::PreviewStatic()
:	m_pListener(NULL)
{
}

PreviewStatic::~PreviewStatic()
{
	m_pListener = NULL;
}


BEGIN_MESSAGE_MAP(PreviewStatic, CStatic)
	//{{AFX_MSG_MAP(PreviewStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void PreviewStatic::OnPaint() 
{
	if (m_pListener != NULL)
	{
		ASSERT(AfxIsValidAddress(m_pListener,sizeof PaintEventListener,TRUE));
		m_pListener->OnPSPaint(*this);
	}
	else
		CStatic::OnPaint();
	
}

BOOL PreviewStatic::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID(pDC);

	if (m_pListener != NULL)
	{
		ASSERT(AfxIsValidAddress(m_pListener,sizeof PaintEventListener,TRUE));
		return m_pListener->OnPSEraseBkgnd(*this,*pDC);
	}
	else
		return CStatic::OnEraseBkgnd(pDC);
}

void PreviewStatic::OnNcPaint() 
{
	if (m_pListener != NULL)
	{
		ASSERT(AfxIsValidAddress(m_pListener,sizeof PaintEventListener,TRUE));
		m_pListener->OnPSNcPaint(*this);
	}
	else
		CStatic::OnNcPaint();
}

void PreviewStatic::SetListener(PaintEventListener* pListener)
{
	m_pListener = pListener;
}


void PreviewStatic::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	Invalidate();
}

void PreviewStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_pListener != NULL)
	{
		ASSERT(AfxIsValidAddress(m_pListener,sizeof PaintEventListener,TRUE));
		m_pListener->OnPSMouseMove(*this,nFlags,point);
	}
	else
		CStatic::OnMouseMove(nFlags, point);
}
