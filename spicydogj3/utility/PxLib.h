#pragma once

namespace PxLib {

void FillRect(CDC& dc, const CRect& rc, COLORREF color);
void DrawEmbossed(CDC& dc, CImageList& il, int i,CPoint p, BOOL bColor=FALSE);
HBITMAP LoadSysColorBitmap(LPCTSTR lpResName, BOOL bMono=FALSE);
HBITMAP LoadSysColorBitmap(UINT nResID, BOOL bMono=FALSE);

};
