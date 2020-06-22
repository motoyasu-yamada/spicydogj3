/////////////////////////////////////////////////////////////////
// MemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support.
//           25 feb 98 - fixed minor assertion bug
//
// This class implements a memory Device Context
/////////////////////////////////////////////////////////////////

#include "STDAFX.H"
#include "MemDC.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//	DIBの水平線のバイト数は４バイト単位でなくてはならない
DWORD GetWidthBytes(DWORD dwWidth,DWORD dwBitsCount)
{
	// ASSERT(0 < dwWidth);
	return	(((dwBitsCount * dwWidth+ (unsigned int)31) / (unsigned int)32) * (unsigned int)4);
}

//	ウインドウ上の有効描画範囲を求める
void GetWndDrawableRect(CWnd* pwnd,CRect& rc)
{
	pwnd->GetClientRect(rc);

	if (pwnd->IsKindOf(RUNTIME_CLASS(CScrollView)))
	{
		CScrollView* pView = (CScrollView*)pwnd;
		CRect rcView (CPoint(0,0),pView->GetTotalSize());

		rc.UnionRect(rc,rcView);
	}
}


MemDC::MemDC(CDC* pDC,LPCRECT prc)
:	CDC()
{
	ASSERT_VALID(pDC);
	if (prc != NULL)
		Init(pDC,prc);
	else
	{
		CRect rcWnd;
		CWnd* pTarget = pDC->GetWindow();
		if (pTarget != NULL)
			GetWndDrawableRect(pTarget,rcWnd);
		else
			rcWnd.SetRectEmpty();
		Init(pDC,rcWnd);
	}
}

MemDC::MemDC(CDC* pDC)
{
	//TRACE("MemDC(CDC* pDC)\n");
	ASSERT_VALID(pDC);

	CRect rcClip;
	VERIFY(pDC->GetClipBox(rcClip));
	CWnd* pTarget = pDC->GetWindow();
	if (pTarget != NULL)
	{
		CRect rcWnd;
		GetWndDrawableRect(pTarget,rcWnd);
		
		rcClip.IntersectRect(rcClip,rcWnd);
	}
	Init(pDC,rcClip);
}


MemDC::MemDC(CDC* pDC,CWnd* pTarget) : CDC()
{
	//TRACE("MemDC(CDC* pDC,CWnd* pTarget)\n");
	ASSERT(pDC != NULL);
	ASSERT_VALID(pDC);
	ASSERT(pTarget != NULL);
	ASSERT_VALID(pTarget);

	CRect rcWnd;
	CRect rcClip;

	VERIFY(pDC->GetClipBox(rcClip));
	GetWndDrawableRect(pTarget,rcWnd);
	
	rcClip.IntersectRect(rcWnd,rcClip);
	
	Init(pDC,rcClip);
}

void MemDC::CreateDIB()
{
	ASSERT_VALID(m_pDC);

	BITMAPINFO bmi;
	BITMAPINFOHEADER& bmih = bmi.bmiHeader;
	bmih.biSize = sizeof BITMAPINFOHEADER;
	bmih.biBitCount = 24;
	bmih.biClrImportant = 0;
	bmih.biClrUsed = 0;
	bmih.biCompression = BI_RGB;
	bmih.biHeight = m_rect.Height();
	bmih.biPlanes = 1;
	bmih.biSizeImage = GetWidthBytes(m_rect.Width(),24) * m_rect.Height();
	bmih.biWidth = m_rect.Width();
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;

	if (bmih.biHeight * bmih.biWidth != 0)
	{
		LPVOID pvBits = NULL;
		m_bitmap.Attach(::CreateDIBSection(*m_pDC,&bmi,DIB_RGB_COLORS,&pvBits,0,0));
		ASSERT(pvBits != NULL);
	}
}

void MemDC::CreateDDB()
{
	int w = m_rect.Width();
	int h = m_rect.Height();
	if (w * h != 0)
	{
		m_bitmap.Attach(::CreateCompatibleBitmap(*m_pDC,w,h));
	}
}

void MemDC::Init(CDC* pDC,LPCRECT prc)
{
    ASSERT_VALID(pDC);

    m_pDC = pDC;
	m_rect = *prc;
    m_pOldBitmap = NULL;
	m_bFliped = FALSE;

#ifndef WCE_NO_PRINTING
    m_bMemDC = !pDC->IsPrinting();
#else
    m_bMemDC = FALSE;
#endif
          
    if (m_bMemDC)    // Create a Memory DC
    {
        CreateCompatibleDC(pDC);
		//CreateDIB();
		CreateDDB();

		if (m_bitmap.GetSafeHandle() != NULL)
			m_pOldBitmap = SelectObject(&m_bitmap);

#ifndef _WIN32_WCE
        SetWindowOrg(m_rect.left, m_rect.top);
#endif
    }
    else        // Make a copy of the relevent parts of the current DC for printing
    {
#ifndef WCE_NO_PRINTING
        m_bPrinting = pDC->m_bPrinting;
#endif
        m_hDC       = pDC->m_hDC;
        m_hAttribDC = pDC->m_hAttribDC;
    }
}


//	操作
BOOL MemDC::BitBlt(int xDst,int yDst,int nW,int nH,CBitmap& bmp,int xSrc,int ySrc,DWORD dwRop)
{
	CDC dcSrc;
	VERIFY(dcSrc.CreateCompatibleDC(this));
	CBitmap* pbmpSaved = dcSrc.SelectObject(&bmp);
	ASSERT(pbmpSaved != NULL);

	BOOL b = CDC::BitBlt(xDst,yDst,nW,nH,&dcSrc,xSrc,ySrc,dwRop);

	VERIFY(dcSrc.SelectObject(pbmpSaved) != NULL);

	return b;
}

void MemDC::Flip()
{
	ASSERT(!m_bFliped);
	if (m_bMemDC) 
	{    
		if (m_pOldBitmap != NULL)
		{
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
						  this, m_rect.left, m_rect.top, SRCCOPY);

			//Swap back the original bitmap.
			VERIFY(SelectObject(m_pOldBitmap) != 0);
		}
	} else {
		// All we need to do is replace the DC with an illegal value,
		// this keeps us from accidently deleting the handles associated with
		// the CDC that was passed to the constructor.
		m_hDC = m_hAttribDC = NULL;
	}
	m_bFliped = TRUE;
}

// Destructor copies the contents of the mem DC to the original DC
MemDC::~MemDC()
{
	if (!m_bFliped)
	{
		Flip();
	}
}

// Allow usage as a pointer
MemDC* MemDC::operator->() {return this;}
    
// Allow usage as a pointer
MemDC::operator MemDC*() {return this;}
