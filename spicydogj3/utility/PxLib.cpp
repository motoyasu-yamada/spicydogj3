#include "stdafx.h"
#include "PxLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// Load a bitmap, converting the standard colors.
// Calls AfxLoadSysColorBitmap to do the work.
//
// RGB(0x00, 0x00, 0x00) (black)      --> COLOR_BTNTEXT
// RGB(0x80, 0x80, 0x80) (dark gray)  --> COLOR_3DSHADOW
// RGB(0xC0, 0xC0, 0xC0) (gray)       --> COLOR_3DFACE
// RGB(0xFF, 0xFF, 0xFF) (white)      --> COLOR_3DHILIGHT
// 
HBITMAP PxLib::LoadSysColorBitmap(LPCTSTR lpResName, BOOL bMono)
{
	HINSTANCE hInst = AfxFindResourceHandle(lpResName, RT_BITMAP);
	HRSRC hRsrc = ::FindResource(hInst, lpResName, RT_BITMAP);
	if (hRsrc == NULL)
		return NULL;
	return AfxLoadSysColorBitmap(hInst, hRsrc, bMono);
}

HBITMAP PxLib::LoadSysColorBitmap(UINT nResID, BOOL bMono) {
	return LoadSysColorBitmap(MAKEINTRESOURCE(nResID), bMono);
}

//////////////////
// Shorthand to fill a rectangle with a solid color.
//
void PxLib::FillRect(CDC& dc, const CRect& rc, COLORREF color)
{
	CBrush brush(color);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	dc.PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
	dc.SelectObject(pOldBrush);
}


////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// Function to draw embossed (disabled) bitmaps
////////////////////////////////////////////////////////////////

const COLORREF CWHITE  = RGB(255,255,255);


// This is the "magic" ROP code used to generate the embossed look for
// a disabled button. It's listed in Appendix F of the Win32 Programmer's
// Reference as PSDPxax (!) which is a cryptic reverse-polish notation for
//
// ((Destination XOR Pattern) AND Source) XOR Pattern
//
// which I leave to you to figure out. In the case where I apply it,
// Source is a monochrome bitmap which I want to draw in such a way that
// the black pixels get transformed to the brush color and the white pixels
// draw transparently--i.e. leave the Destination alone.
//
// black ==> Pattern
// white ==> Destintation (ie, transparent)
//
// 0xb8074a is the ROP code that does this. For more info, see Charles
// Petzold, _Programming Windows_, 2nd Edition, p 622-624.
//
const DWORD		MAGICROP		= 0xb8074a;

//////////////////
// Draw an image with the embossed (disabled) look.
//
//		dc			device context to draw in
//		il			image list containing image
//		i			index of image to draw
//		p			point in dc to draw image at
//    bColor	do color embossing. Default is B/W.
//
void PxLib::DrawEmbossed(CDC& dc, CImageList& il, int i,
	CPoint p, BOOL bColor)
{
	IMAGEINFO info;
	VERIFY(il.GetImageInfo(0, &info));
	CRect rc = info.rcImage;
	int cx = rc.Width();
	int cy = rc.Height();

	// create memory dc
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);

	// create mono or color bitmap
	CBitmap bm;
	if (bColor)
		bm.CreateCompatibleBitmap(&dc, cx, cy);
	else
		bm.CreateBitmap(cx, cy, 1, 1, NULL);

	// draw image into memory DC--fill BG white first
	CBitmap* pOldBitmap = memdc.SelectObject(&bm);
	memdc.PatBlt(0, 0, cx, cy, WHITENESS);
	il.Draw(&memdc, i, CPoint(0,0), ILD_TRANSPARENT);

	// This seems to be required. Why, I don't know. ???
	COLORREF colorOldBG = dc.SetBkColor(CWHITE);

	// Draw using hilite offset by (1,1), then shadow
	CBrush brShadow(GetSysColor(COLOR_3DSHADOW));
	CBrush brHilite(GetSysColor(COLOR_3DHIGHLIGHT));
	CBrush* pOldBrush = dc.SelectObject(&brHilite);
	dc.BitBlt(p.x+1, p.y+1, cx, cy, &memdc, 0, 0, MAGICROP);
	dc.SelectObject(&brShadow);
	dc.BitBlt(p.x, p.y, cx, cy, &memdc, 0, 0, MAGICROP);
	dc.SelectObject(pOldBrush);
	dc.SetBkColor(colorOldBG);				 // restore
	memdc.SelectObject(pOldBitmap);		 // ...
}
