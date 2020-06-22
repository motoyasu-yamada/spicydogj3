#include "stdafx.h"
#include "PaintMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

PaintMemDC::PaintMemDC(CWnd* pWnd)
:	m_dcPaint(pWnd),
	m_bFlipped(FALSE),
	m_bMemed(FALSE),
	m_pbmpSaved(NULL) {

	ASSERT_VALID(pWnd);
	pWnd->GetClientRect(m_rc);

	if (!CreateCompatibleDC(&m_dcPaint)) {
		AfxThrowResourceException();
	}
	m_hAttribDC = m_dcPaint.GetSafeHdc();

	const int w = m_rc.Width();
	const int h = m_rc.Height();
	if (w * h != 0) {
		if (!m_bmpMem.Attach(::CreateCompatibleBitmap(m_dcPaint,w,h))){
			AfxThrowResourceException();
		}
		m_bMemed = TRUE;
	}

	if (m_bMemed) {
		m_pbmpSaved = SelectObject(&m_bmpMem);
		ASSERT(m_pbmpSaved != NULL);
	}
    SetWindowOrg(m_rc.left, m_rc.top);
}

PaintMemDC::~PaintMemDC() {
	if (m_bMemed && !m_bFlipped) {
		Flip();
	}
	if (m_pbmpSaved != NULL) {
		VERIFY(SelectObject(m_pbmpSaved) != 0);
	}
}

void PaintMemDC::Flip() {
	if (!m_bMemed) 
		return;  
	ASSERT_VALID(this);
	ASSERT_VALID(&m_dcPaint);
	ASSERT_VALID(&m_bmpMem);
	ASSERT_VALID(m_pbmpSaved);

	int nSaved = SaveDC();
	SetViewportOrg(CPoint(0,0));

	m_dcPaint.BitBlt(m_rc.left,m_rc.top,m_rc.Width(),m_rc.Height(),this,
		             m_rc.left,m_rc.top,SRCCOPY);

	RestoreDC(nSaved);

	m_bFlipped = TRUE;
}

void PaintMemDC::SetViewportOrg(CPoint pt) {
	CDC::SetViewportOrg(pt);
	//m_dcPaint.SetViewportOrg(pt);
}