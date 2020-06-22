#include "stdafx.h"
#include "resource.h"
#include "EditColoringSelectDlg.h"
#include "EditColoringDlg.h"
#include "model/stage/Coloring.h"
#include "dlg/ColorPaletteDlg.h"
#include "utility/ChildDataExchange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(EditColoringSelectDlg, CDialog)
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_CLRBOXEX_SELECT,OnSelect)
END_MESSAGE_MAP()

EditColoringSelectDlg::EditColoringSelectDlg(Coloring* coloring)
:	m_coloring(coloring) {
	m_apply = 0;
	ASSERT_POINTER(m_coloring,Coloring);
}

void EditColoringSelectDlg::DoDataExchange(CDataExchange* pDX) {
	ASSERT_POINTER(&m_coloring,Coloring);
	ChildDataExchange cde(this,pDX);
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDCS_APPLY, m_applySlider);
	DDX_Slider(pDX, IDCS_APPLY, m_apply);
	ColorBoxEx::DDX_Control(pDX,IDCC_SELCLR,m_colorBox,IDB_LINE,RGB(0x00,0x80,0x80));

	if (pDX->m_bSaveAndValidate) {
		m_coloring->mulRed   = 
		m_coloring->mulGreen =
		m_coloring->mulBlue  = static_cast<double>(100 - m_apply) / 100;
		double d = static_cast<double>(m_apply) / 100;
		COLORREF c = m_colorBox.GetColor();
		m_coloring->addRed   = GetRValue(c) * d;
		m_coloring->addGreen = GetGValue(c) * d;
		m_coloring->addBlue  = GetBValue(c) * d;

	} else {
		double dApply;
		if (m_coloring->mulRed == m_coloring->mulGreen &&
			m_coloring->mulBlue == m_coloring->mulGreen) {
			dApply = 1 - m_coloring->mulRed;
		} else {
			dApply = 1;
		}
		m_apply = dApply * 100;
		m_applySlider.SetPos(m_apply);

		if (m_colorBox.GetSafeHwnd() != NULL)
			m_colorBox.SetColor(RGB(m_coloring->addRed / dApply,
							   m_coloring->addGreen / dApply,
							   m_coloring->addBlue / dApply));

		m_applySlider.RedrawWindow();
	}
}

BOOL EditColoringSelectDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_applySlider.SetRange(0,100);
	m_applySlider.SetTicFreq(25);
	m_colorBox.SetColor(RGB(0xFF,00,00));
	
	return TRUE;
}

LRESULT EditColoringSelectDlg::OnSelect(WPARAM,LPARAM lparam) {
	COLORREF* color = reinterpret_cast<COLORREF*>(lparam);
	CRect rc;
	m_colorBox.GetWindowRect(rc);
	ColorPaletteDlg dlg(*color,FALSE,rc.left,rc.bottom);
	if (dlg.DoModal() == IDOK) {
		*color = dlg.GetColor();
		m_colorBox.SetColor(*color);
		UpdateData(TRUE);
		EditColoringDlg::Invalidate(this);
		return TRUE;
	} else {
		return FALSE;
	}
}

void EditColoringSelectDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	UpdateData(TRUE);
	EditColoringDlg::Invalidate(this);
}
