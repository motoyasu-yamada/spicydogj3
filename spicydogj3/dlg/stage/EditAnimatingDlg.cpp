#include "stdafx.h"
#include "resource.h"
#include "EditAnimatingDlg.h"
#include "utility/StringUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(EditAnimatingDlg, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

EditAnimatingDlg::EditAnimatingDlg(BOOL resizable,Animating* animating,CWnd* parent)
:	CDialog(IDD_EDIT_ANIMATING, parent) {
	m_orgAnimating = animating;
	m_resizable    = resizable;
	editAnimating  = *animating;
}

void EditAnimatingDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDCE_ACCENT,m_editAccent);
	DDX_Control(pDX,IDCS_ACCENT,m_scAccent);

	DDX_Slider (pDX,IDCS_ACCENT,editAnimating.easing);
	DDX_Check(pDX, IDCC_TWEEN,  editAnimating.animating);

	if (!pDX->m_bSaveAndValidate) {
		m_editAccent.SetWindowText(FormatString("%d",editAnimating.easing));
	}
}

void EditAnimatingDlg::OnOK() {
	if (UpdateData(TRUE)) {
		CDialog::OnOK();
	}
}

BOOL EditAnimatingDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_scAccent.SetRange(-100,100);
	m_scAccent.SetTicFreq(50);
	m_scAccent.SetPos(editAnimating.easing);

	return TRUE;
}

void EditAnimatingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);	
	m_editAccent.SetWindowText(FormatString("%d",m_scAccent.GetPos()));
}
