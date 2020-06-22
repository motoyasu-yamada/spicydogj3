#include "stdafx.h"
#include "resource.h"
#include "SetSoftkeyActionDlg.h"

#include "model/action/SetSoftkeyAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(SetSoftkeyActionDlg,ActionDlg)
BEGIN_MESSAGE_MAP(SetSoftkeyActionDlg, ActionDlg)
END_MESSAGE_MAP()

SetSoftkeyActionDlg::SetSoftkeyActionDlg() {
}


void SetSoftkeyActionDlg::DoDataExchange(CDataExchange* pDX) {
	DDX_Text(pDX, IDCE_LABEL, m_label);
	DDV_MaxChars(pDX,m_label, 4);
	DDX_Radio(pDX, IDCR_SOFTKEY1, m_which);
}

void SetSoftkeyActionDlg::SetAction(Appli* appli,Action* action) {
	ActionDlg::SetAction(appli,action);
	m_which = GetAction()->GetWhich();
	m_label = GetAction()->GetLabel();
	UpdateData(FALSE);
}

void SetSoftkeyActionDlg::Apply() {
	UpdateData(TRUE);
	GetAction()->SetSoftkey(m_which,m_label);
}

BOOL SetSoftkeyActionDlg::Create(CWnd* wnd) {
	ASSERT_VALID(wnd);
	return CDialog::Create(IDD_ACTDLG_SET_SOFTKEY,wnd);
}

SetSoftkeyAction* SetSoftkeyActionDlg::GetAction() {
	SetSoftkeyAction* action = dynamic_cast<SetSoftkeyAction*>(m_action);
	ASSERT_VALID(action);
	return action;
}

