#pragma once
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

class MemDC : public CDC
{
private:
    CBitmap m_bitmap;
    CBitmap* m_pOldBitmap;
    CDC* m_pDC;
    CRect m_rect;
    BOOL m_bMemDC;
	BOOL m_bFliped;

public:
	MemDC(CDC* pDC,LPCRECT prc);
	MemDC(CDC* pDC);
    MemDC(CDC* pDC,CWnd* pTarget);
	void Flip();
	BOOL BitBlt(int xDst,int yDst,int nW,int nH,CBitmap& bmp,int xSrc,int ySrc,DWORD dwRop);

public:
    ~MemDC();

    MemDC* operator->();
    operator MemDC*();

	void Init(CDC* pdc,LPCRECT prc);

	void CreateDDB();
	void CreateDIB();
};

//{{AFX_INSERT_LOCATION}}