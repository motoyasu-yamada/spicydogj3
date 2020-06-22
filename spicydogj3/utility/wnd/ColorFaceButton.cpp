#include "stdafx.h"
#include "resource.h"
#include "ColorFaceButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(ColorFaceButton, CButton)
	//{{AFX_MSG_MAP(ColorFaceButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

ColorFaceButton::ColorFaceButton()
:	m_cr(RGB(0xFF,0xFF,0xFF)) {
}

ColorFaceButton::~ColorFaceButton() {
}

COLORREF ColorFaceButton::GetColor() const {
	return m_cr;
}

void ColorFaceButton::SetColor(COLORREF cr) {
	m_cr = cr;
	Invalidate();
}

void ColorFaceButton::DrawItem(LPDRAWITEMSTRUCT pDIS) {
	ASSERT_POINTER(pDIS,PDRAWITEMSTRUCT);

	CRect rcBtn(pDIS->rcItem);
	CDC* pDC = CDC::FromHandle(pDIS->hDC);
	ASSERT_VALID(pDC);

	BOOL bDown     = pDIS->itemState & ODS_SELECTED;
	BOOL bDisabled = pDIS->itemState & ODS_DISABLED;
	COLORREF crLT  = ::GetSysColor(bDown ? COLOR_BTNSHADOW    : COLOR_BTNHIGHLIGHT);
	COLORREF crRB  = ::GetSysColor(bDown ? COLOR_BTNHIGHLIGHT : COLOR_BTNSHADOW);

	pDC->Draw3dRect(rcBtn,crLT,crRB);
	rcBtn.DeflateRect(1,1);
	pDC->Draw3dRect(rcBtn,crLT,crRB);

	if (!bDisabled) {
		rcBtn.DeflateRect(3,3);
		pDC->FillSolidRect(rcBtn,RGB(0x00,0x00,0x00));
		rcBtn.DeflateRect(1,1);
		pDC->FillSolidRect(rcBtn,m_cr);
	} else {
		rcBtn.DeflateRect(3,3);
		pDC->FillSolidRect(rcBtn,RGB(0xFF,0xFF,0xFF));
		rcBtn.DeflateRect(1,1);
		pDC->FillSolidRect(rcBtn,RGB(0x80,0x80,0x80));
	}
}

void ColorFaceButton::OnClicked() {
	CColorDialog dlg(m_cr);
	if (dlg.DoModal()) {
		SetColor(dlg.GetColor());
		CWnd* pParent = GetParent();
		if (pParent != NULL) {
			ASSERT_VALID(pParent);
			pParent->SendMessage(WM_COMMAND,GetDlgCtrlID());
		}
	}
}
