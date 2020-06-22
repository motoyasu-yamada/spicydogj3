#include "stdafx.h"
#include "LeadToolUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void TransparentStretchBitBlt(HDC hDC,int x1,int y1,int w1,int h1,HBITMAP hbm,int x2, int y2,int w2,int h2,COLORREF crTrans);
void TransparentStretchBitBlt(HDC hDC,int x1,int y1,int w1,int h1,HBITMAP hbm,int x2, int y2,int w2,int h2,BOOL bUseTrans,COLORREF crTrans);

/**
	LeadTools用のヘルパーライブラリ

	@author		m.yamada
	@version	1.0	2001/08/01
*/
//	指定したビットマップをリサイズしたビットマップを作成
BOOL LBitmap_Resize(const LBitmapBase& s,LBitmapBase& dst,int cx,int cy) {
	LBitmapBase& src = const_cast<LBitmapBase&>(s);

	RGBQUAD colors[256];
	src.GetColors(0,255,colors);

	if (dst.Create(cx,cy,24,ORDER_BGR,colors,TOP_LEFT,TYPE_CONV) != SUCCESS)
		return FALSE;

	if (src.Resize(&dst) != SUCCESS)
		return FALSE;

	return TRUE;
}

//	ビットマップをコピーする
BOOL LBitmap_Copy(const LBitmapBase& src,LBitmapBase& dst)
{
	return dst.Copy(const_cast<LBitmapBase&>(src)) == SUCCESS;
}
//	ビットマップサイズを取得する
int LBitmap_GetWidth(const LBitmapBase& bmp)
{
	return const_cast<LBitmapBase&>(bmp).GetWidth();
}
int LBitmap_GetHeight(const LBitmapBase& bmp)
{
	return const_cast<LBitmapBase&>(bmp).GetHeight();
}

/**
	LeadToolsのBITMAP形式のデータを透明色を
	適用して描画する。
*/
L_VOID LPaintTransparent
(
	HDC hDC,
	int nDstX,
	int nDstY,
	int nDstW,
	int nDstH,
	const LBitmapBase& _lbmp,
	int nSrcX,
	int nSrcY,
	int nSrcW,
	int nSrcH,
	BOOL b,
	COLORREF cr
)
{
	LBitmapBase& lbmp = const_cast<LBitmapBase&>(_lbmp);

	CWnd* pwndDesktop = CWnd::GetDesktopWindow();
	ASSERT(pwndDesktop != NULL);
	CClientDC dcDesktop(pwndDesktop);

	HPALETTE h = lbmp.GetPalette();
	HPALETTE hSaved = NULL;
	if (h != NULL)
	{
		hSaved = ::SelectPalette(hDC,h,TRUE);
		ASSERT(hSaved != NULL);
		VERIFY(::RealizePalette(hDC) != GDI_ERROR);
	}

	CBitmap bmp;
	VERIFY(bmp.Attach(lbmp.ConvertToDDB(dcDesktop)));
	ASSERT_VALID(&bmp);
	TransparentStretchBitBlt(hDC,nDstX,nDstY,nDstW,nDstH,bmp,nSrcX,nSrcY,nSrcW,nSrcH,b,cr);

	if (h != NULL)
		VERIFY(::SelectPalette(hDC,hSaved,TRUE));
}

/**
	透過色を利用した描画
*/

void TransparentStretchBitBlt
(
	HDC hDC, 
	int x1, 
	int y1, 
	int w1,
	int h1,
	HBITMAP hbm,
	int x2, 
	int y2, 
	int w2,
	int h2,
	COLORREF crTrans
)
{
	CWnd* pwndDesktop = CWnd::GetDesktopWindow();
	ASSERT(pwndDesktop != NULL);
	CClientDC dcDesktop(pwndDesktop);

    COLORREF cr;
    HBITMAP hbmAndBk = NULL, hbmAndObj = NULL, hbmAndMem = NULL, hbmSrc =NULL;
    HDC hAndMemDC = NULL, hAndBackDC = NULL, hAndObjDC = NULL, hSrcDC = NULL, hTempDC = NULL;
    POINT pts;

    if(!hbm)
        return;

    if((hTempDC = CreateCompatibleDC(dcDesktop)) == NULL)
        goto transsblt_end;
    VERIFY(SelectObject(hTempDC, hbm) != NULL);

    if((hSrcDC = CreateCompatibleDC(dcDesktop))  == NULL)
        goto transsblt_end;
    if((hbmSrc = CreateCompatibleBitmap(dcDesktop, w1, h1)) == NULL)
        goto transsblt_end;
    VERIFY(SelectObject(hSrcDC, hbmSrc)!= NULL);

    StretchBlt(hSrcDC, 0, 0, w1, h1, hTempDC, x2, y2, w2, h2, SRCCOPY);

    DeleteDC(hTempDC);
    hTempDC = NULL;

    pts.x = w1;
    pts.y = h1;
    DPtoLP(hSrcDC, &pts, 1);

    if((hAndBackDC   = CreateCompatibleDC(dcDesktop)) == NULL)
        goto transsblt_end;
    if((hAndObjDC = CreateCompatibleDC(dcDesktop)) == NULL)
        goto transsblt_end;
    if((hAndMemDC    = CreateCompatibleDC(dcDesktop)) == NULL)
        goto transsblt_end;

    if((hbmAndBk   = CreateBitmap(pts.x, pts.y, 1, 1, NULL)) == NULL)
        goto transsblt_end;
    if((hbmAndObj = CreateBitmap(pts.x, pts.y, 1, 1, NULL)) == NULL)
        goto transsblt_end;
    if((hbmAndMem    = CreateCompatibleBitmap(dcDesktop, pts.x, pts.y)) == NULL)
        goto transsblt_end;

    SelectObject(hAndBackDC, hbmAndBk);
    SelectObject(hAndObjDC, hbmAndObj);
    SelectObject(hAndMemDC, hbmAndMem);

    SetMapMode(hSrcDC, dcDesktop.GetMapMode());

    cr = SetBkColor(hSrcDC, crTrans);

    BitBlt(hAndObjDC, 0, 0, pts.x, pts.y, hSrcDC, 0, 0, SRCCOPY);

    SetBkColor(hSrcDC, cr);

    BitBlt(hAndBackDC, 0, 0, pts.x, pts.y, hAndObjDC, 0, 0, NOTSRCCOPY);

    BitBlt(hAndMemDC, 0, 0, pts.x, pts.y, hDC, x1, y1, SRCCOPY);

    BitBlt(hAndMemDC, 0, 0, pts.x, pts.y, hAndObjDC, 0, 0, SRCAND);

    BitBlt(hSrcDC, 0, 0, pts.x, pts.y, hAndBackDC, 0, 0, SRCAND);

    BitBlt(hAndMemDC, 0, 0, pts.x, pts.y, hSrcDC, 0, 0, SRCPAINT);

    BitBlt(hDC, x1, y1, pts.x, pts.y, hAndMemDC, 0, 0, SRCCOPY);

transsblt_end:

    if(hTempDC)
        DeleteDC(hTempDC);
    if(hAndMemDC)
        DeleteDC(hAndMemDC);
    if(hAndBackDC)
        DeleteDC(hAndBackDC);
    if(hAndObjDC)
        DeleteDC(hAndObjDC);
    if(hSrcDC)
        DeleteDC(hSrcDC);

    if(hbmAndBk)
        DeleteObject(hbmAndBk);
    if(hbmAndObj)
        DeleteObject(hbmAndObj);
    if(hbmAndMem)
        DeleteObject(hbmAndMem);
    if(hbmSrc)
        DeleteObject(hbmSrc);
}

void TransparentStretchBitBlt
(
	HDC hDC, 
	int x1, 
	int y1, 
	int w1,
	int h1,
	HBITMAP hbm,
	int x2, 
	int y2, 
	int w2,
	int h2,
	BOOL bUseTrans,
	COLORREF crTrans
)
{
	if (bUseTrans) {
		TransparentStretchBitBlt(hDC,x1,y1,w1,h1,hbm,x2,y2,w2,h2,crTrans);
	} else {
		CDC* pdcDst = CDC::FromHandle(hDC);
		ASSERT(pdcDst != NULL);

		CWnd* pwndDesktop = CWnd::GetDesktopWindow();
		ASSERT(pwndDesktop != NULL);
		CClientDC dcDesktop(pwndDesktop);

		CDC dcMem;
		VERIFY(dcMem.CreateCompatibleDC(&dcDesktop));
		CBitmap* saved = dcMem.SelectObject(CBitmap::FromHandle(hbm));
		ASSERT_VALID(saved);
		pdcDst->SetStretchBltMode(COLORONCOLOR);
		pdcDst->StretchBlt(x1,y1,w1,h1,&dcMem,x2,y2,w2,h2,SRCCOPY);
		VERIFY(dcMem.SelectObject(saved) != NULL);
		return;
	}
}
