#include "stdafx.h"
#include "resource.h"
#include "BuildBar.h"

#include "model/build/BuildOutput.h"
#include "utility/StringUtility.h"
#include "SpicydogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int PROGRESSBAR_CX = 100;
const int ICON_CX = 16;
const int MARGIN = 4;
const int TOTAL_CX = ICON_CX + MARGIN + PROGRESSBAR_CX;

const COLORREF COLOR_NORMAL   = RGB(0x00,0x00,0x80);
const COLORREF COLOR_SIZEWARN = RGB(0x80,0x80,0x00);
const COLORREF COLOR_OVERSIZE = RGB(0x80,0x00,0x00);
const COLORREF COLOR_WARNING  = RGB(0x80,0x80,0x00);
const COLORREF COLOR_IMAGEMASK= RGB(0x00,0x80,0x80);

CFont BuildBar::s_font;
CImageList BuildBar::s_images;

IMPLEMENT_BUTTON(BuildBar);
BuildBar::BuildBar()
:	m_State(BuildOutput::STATE_NONE) {
	DocumentNotifier::GetNotifier()->AddListener(this);
}

void BuildBar::clinit() {
	LOGFONT logfont;
	if (!SystemParametersInfo(SPI_GETICONTITLELOGFONT,sizeof logfont,&logfont,0)) {
		ASSERT(FALSE);
		return;
	}
	VERIFY(s_font.CreateFontIndirect(&logfont));
	VERIFY(s_images.Create(IDB_BUILDBAR_ICON,ICON_CX,1,COLOR_IMAGEMASK));
}

BuildBar::~BuildBar()
{
	DocumentNotifier::GetNotifier()->RemoveListener(this);
}

void BuildBar::DrawIcon(CDC& dc,CRect& rc,int n) {
	CPoint pt(rc.left,(rc.top+rc.bottom - ICON_CX) / 2);
	s_images.Draw(&dc,n,pt,ILD_NORMAL);
}

void BuildBar::DrawMet(CDC& dc,CRect& rc,int nSize,int nMax,COLORREF cr) {
	DrawMetBkgnd(dc,rc);
	CRect rcMet = rc;
	rcMet.right = rc.left + rc.Width() * nSize / nMax;
	dc.FillSolidRect(rcMet,cr);

	CFont* saved = dc.SelectObject(&s_font);
	ASSERT_VALID(saved);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0xFF,0xFF,0xFF));
	dc.DrawText(FormatString("%d/%d",nSize,nMax),rc,DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	VERIFY(dc.SelectObject(saved) != NULL);
}

void BuildBar::DrawMetBkgnd(CDC& dc,CRect& rc) {
	dc.Draw3dRect(rc,GetSysColor(COLOR_3DDKSHADOW),GetSysColor(COLOR_3DHILIGHT));
	rc.DeflateRect(1,1);
	dc.FillSolidRect(rc,GetSysColor(COLOR_APPWORKSPACE));
}

void BuildBar::DrawErrMet(CDC& dc,CRect& rc,UINT nID) {
	DrawMetBkgnd(dc,rc);
	CString str;
	VERIFY(str.LoadString(nID));
	
	CFont* saved = dc.SelectObject(&s_font);
	ASSERT_VALID(saved);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0xFF,0xFF,0xFF));
	dc.DrawText(str,rc,DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	VERIFY(dc.SelectObject(saved) != NULL);
}

void BuildBar::OnDocumentActivated(CDocument* p) {
	OnNotify(SpicydogDoc::EVENTNOTIFYCODE_BUILT,p,0,0);
}

void BuildBar::OnBuilt(DWORD dwNotifyCode,CDocument* p) {
	if (dwNotifyCode != SpicydogDoc::EVENTNOTIFYCODE_BUILT) {
		return;
	}
	if (p == NULL) {
		m_State = BuildOutput::STATE_NONE;
		return;
	}

	SpicydogDoc* document = DYNAMIC_DOWNCAST(SpicydogDoc,p);
	ASSERT_VALID(document);
	const BuildOutput& bo = document->buildOutput;
	m_State        = bo.GetResult();
	m_nDataSize    = bo.GetJarSize();
	m_nMaxDataSize = bo.GetMaxJarSize();
}

void BuildBar::OnNotify(DWORD dwNotifyCode,CDocument* p,LPARAM,LPVOID) {
	OnBuilt(dwNotifyCode,p);
	Invalidate(FALSE);
}

void BuildBar::AdjustSize() {
	SECStdBtn::AdjustSize();
	m_cx = TOTAL_CX;
}

void BuildBar::DrawButton(CDC& dc,SECBtnDrawData& data) {
	CRect rc;
	GetBtnRect(rc);
	CRect rcIcon(rc.left,rc.top,rc.left+ICON_CX,rc.bottom);
	CRect rcMet (rc.right-PROGRESSBAR_CX,rc.top,rc.right,rc.bottom);
	switch(m_State) {
	case BuildOutput::STATE_NONE:
		DrawIcon(dc,rcIcon,0);
		DrawErrMet(dc,rcMet,IDS_EN_MET_NONE);
		break;
	case BuildOutput::STATE_NORMAL:
		DrawIcon(dc,rcIcon,1);
		DrawMet(dc,rcMet,m_nDataSize,m_nMaxDataSize,(m_nDataSize < (m_nMaxDataSize * 0.8)) ? COLOR_NORMAL : COLOR_SIZEWARN);
		break;
	case BuildOutput::STATE_WARNING:
		DrawIcon(dc,rcIcon,2);
		DrawMet(dc,rcMet,m_nDataSize,m_nMaxDataSize,COLOR_WARNING);
		break;
	case BuildOutput::STATE_OVERSIZE:
		DrawIcon(dc,rcIcon,3);
		DrawMet(dc,rcMet,m_nDataSize,m_nMaxDataSize,COLOR_OVERSIZE);
		break;
	case BuildOutput::STATE_ERROR:
		DrawIcon(dc,rcIcon,4);
		DrawErrMet(dc,rcMet,IDS_EN_MET_APPERR);
		break;
	}
}