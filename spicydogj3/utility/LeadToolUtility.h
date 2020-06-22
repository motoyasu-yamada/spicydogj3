#pragma once
#include <ltwrappr.h>

void LPaintTransparent
(
	HDC hDC,
	int nDstX,
	int nDstY,
	int nDstW,
	int nDstH,
	const LBitmapBase& lbmp,
	int nSrcX,
	int nSrcY,
	int nSrcW,
	int nSrcH,
	BOOL bUseTransparent,
	COLORREF crTransparent = 0
);

BOOL LBitmap_Resize(const LBitmapBase& src,LBitmapBase& dst,int cx,int cy);
BOOL LBitmap_Copy(const LBitmapBase& src,LBitmapBase& dst);
int LBitmap_GetWidth(const LBitmapBase&);
int LBitmap_GetHeight(const LBitmapBase&);