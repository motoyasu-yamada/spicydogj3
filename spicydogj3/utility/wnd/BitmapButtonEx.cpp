#include "stdafx.h"
#include "BitmapButtonEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(BitmapButtonEx, CButton)
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

BitmapButtonEx::BitmapButtonEx()
:	m_nWidth(0)
{
}

BitmapButtonEx::~BitmapButtonEx()
{
}

void BitmapButtonEx::DrawItem(LPDRAWITEMSTRUCT pDIS) 
{
	ASSERT_VALID(&m_bmp);
	CDC* pDC = CDC::FromHandle(pDIS->hDC);
	ASSERT_VALID(pDC);
	
	CDC dcSrc;
	VERIFY(dcSrc.CreateCompatibleDC(pDC));

	CBitmap* saved = dcSrc.SelectObject(&m_bmp);
	ASSERT(saved != NULL);

	UINT nState = pDIS->itemState;
	CRect rc = pDIS->rcItem;

	if (nState & ODS_DISABLED) {
		pDC->BitBlt(rc.left,rc.top,m_nWidth,m_nHeight,&dcSrc,m_nSrcX + m_nWidth * DisableState,m_nSrcY,SRCCOPY);
	} else if (nState & ODS_SELECTED) {
		pDC->BitBlt(rc.left,rc.top,m_nWidth,m_nHeight,&dcSrc,m_nSrcX + m_nWidth * DownState,m_nSrcY,SRCCOPY);
	} else {
		pDC->BitBlt(rc.left,rc.top,m_nWidth,m_nHeight,&dcSrc,m_nSrcX + m_nWidth * NormalState,m_nSrcY,SRCCOPY);
	}

	VERIFY(dcSrc.SelectObject(saved) == &m_bmp);
}


BOOL BitmapButtonEx::LoadBitmap(UINT n,int nWidth)
{
	if (!m_bmp.LoadBitmap(n))
		return FALSE;
	
	BITMAP bm;
	VERIFY(m_bmp.GetBitmap(&bm));
	m_nHeight = bm.bmHeight;
	m_nSrcX = 0;
	m_nSrcY = 0;
	if (nWidth == -1)
		m_nWidth = m_nHeight;
	else
		m_nWidth = nWidth;

	ASSERT(0 < m_nWidth && m_nWidth * STATE_COUNT <= bm.bmWidth);

	SetWindowPos(NULL,0,0,m_nWidth,m_nHeight,SWP_NOZORDER | SWP_NOMOVE);

	return TRUE;
}

UINT BitmapButtonEx::OnGetDlgCode() 
{
	return DLGC_BUTTON;
}
