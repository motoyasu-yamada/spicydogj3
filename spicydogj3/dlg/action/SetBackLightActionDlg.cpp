#include "stdafx.h"
#include "resource.h"
#include "SetBacklightActionDlg.h"
#include "model/action/SetBacklightAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(SetBacklightActionDlg,ActionDlg)
BEGIN_MESSAGE_MAP(SetBacklightActionDlg, ActionDlg)
END_MESSAGE_MAP()

SetBacklightActionDlg::SetBacklightActionDlg() {
	m_enabled = true;
}

//m_enabled

void SetBacklightActionDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX,IDCR_OFF,m_enabled);
}

void SetBacklightActionDlg::SetAction(Appli* appli,Action* action) {
	ActionDlg::SetAction(appli,action);
	m_enabled = GetAction()->GetEnabled() ? 1 : 0;
	UpdateData(FALSE);
}

void SetBacklightActionDlg::Apply() {
	UpdateData(TRUE);
	GetAction()->SetEnabled(m_enabled == 1);
}

BOOL SetBacklightActionDlg::Create(CWnd* wnd) {
	ASSERT_VALID(wnd);
	return CDialog::Create(IDD_ACTDLG_SET_BACKLIGHT,wnd);
}

SetBacklightAction* SetBacklightActionDlg::GetAction() {
	SetBacklightAction* action = dynamic_cast<SetBacklightAction*>(m_action);
	ASSERT_VALID(action);
	return action;
}
