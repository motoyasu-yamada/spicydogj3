#include "stdafx.h"
#include "FontStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(FontStatic, CStatic)
	//{{AFX_MSG_MAP(FontStatic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

FontStatic::FontStatic()
{
}

FontStatic::~FontStatic()
{
}

void FontStatic::GetLogFont(LOGFONT& l)
{
	CFont* pf = GetFont();
	ASSERT_VALID(pf);
	if (pf != NULL)
		VERIFY(pf->GetLogFont(&l));
}

void FontStatic::SetLogFont(const LOGFONT& lf)
{
	CFont* pf = new CFont;
	ASSERT_VALID(pf);
	VERIFY(pf->CreateFontIndirect(&lf));
	SetFont(pf,FALSE);
	m_pFont = FONTPTR(pf);
}

void FontStatic::AdjustSize()
{
	CWindowDC dc(this);
	{
		CFont* saved = dc.SelectObject(m_pFont.get());
		ASSERT_VALID(saved);
		CString str;
		GetWindowText(str);
		CSize sz = dc.GetTextExtent(str);
		VERIFY(SetWindowPos(NULL,0,0,sz.cx,sz.cy,SWP_NOMOVE | SWP_NOZORDER));
		VERIFY(dc.SelectObject(saved) != NULL);
	}
}