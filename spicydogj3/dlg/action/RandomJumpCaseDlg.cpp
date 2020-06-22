#include "stdafx.h"
#include "resource.h"
#include "RandomJumpCaseDlg.h"

#include "model/Appli.h"
#include "model/action/RandomJumpAction.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RandomJumpCaseDlg::RandomJumpCaseDlg(Appli* appli,RandomJumpCase* rcase,CWnd* wnd)
:	CDialog(IDD_ACTDLG_RANDOM_JUMP_CASE, wnd) {
	ASSERT_VALID(appli);
	ASSERT_VALID(rcase);
	ASSERT_VALID(wnd);

	appli->GetLabelList(m_labels);
	m_case        = rcase;
	m_probability = m_case->GetProbability();
}

void RandomJumpCaseDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCC_LABEL_LIST, m_cbLabelList);
	DDX_Text(pDX, IDCE_PROBABILITY, m_probability);
}

BEGIN_MESSAGE_MAP(RandomJumpCaseDlg, CDialog)
	ON_CBN_SELCHANGE(IDCC_LABEL_LIST, OnChanged)
	ON_EN_CHANGE(IDCE_PROBABILITY, OnChanged)
END_MESSAGE_MAP()

void RandomJumpCaseDlg::OnOK() {
	LABEL_NAME_LIST::const_iterator i;
	int selected;

	if (!UpdateData(TRUE)) {
		ASSERT(FALSE);
		goto error;
	}
	
	selected = m_cbLabelList.GetCurSel();
	if (selected == CB_ERR) {
		ASSERT(FALSE);
		goto error;
	}
	
	i = m_labels.begin();
	std::advance(i,selected);

	m_case->SetCase(m_probability,i->first);
	
	CDialog::OnOK();
	return;

error:
	AfxMessageBox(IDC_EN_RANDOM_JUMP_INVALID_CASE,MB_OK | MB_ICONSTOP);
	return;
}

void RandomJumpCaseDlg::OnChanged() {
	UpdateCtrls();
}

void RandomJumpCaseDlg::UpdateCtrls() {
	BOOL enabled = TRUE;
	BOOL translated;
	
	if (m_cbLabelList.GetCurSel() == CB_ERR) {
		enabled = FALSE;
	}
	int probability = GetDlgItemInt(IDCE_PROBABILITY,&translated,FALSE);
	if (probability < RandomJumpAction::MIN_CASE_VAL) {
		enabled = FALSE;
	}
	if (RandomJumpAction::MAX_CASE_VAL < probability) {
		enabled = FALSE;
	}
	if (!translated) {
		enabled = FALSE;
	}
	
	EnableDlgItem(this,IDOK,enabled);
}

BOOL RandomJumpCaseDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	Label::SetupLabelComboBox(m_cbLabelList,m_labels,m_case->GetLabel());
	UpdateCtrls();
	return TRUE;
}
