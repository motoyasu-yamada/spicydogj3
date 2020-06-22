#include "stdafx.h"
#include "resource.h"
#include "SetVibrationActionDlg.h"

#include "model/action/SetVibrationAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(SetVibrationActionDlg,ActionDlg)
BEGIN_MESSAGE_MAP(SetVibrationActionDlg, ActionDlg)
	//{{AFX_MSG_MAP(SetVibrationActionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

SetVibrationActionDlg::SetVibrationActionDlg() {
	m_vibration = 1;
}

void SetVibrationActionDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDCR_OFF, m_vibration);
}

void SetVibrationActionDlg::SetAction(Appli* appli,Action* action) {
	ActionDlg::SetAction(appli,action);
	m_vibration = GetAction()->GetVibration() ? 1 : 0;
	UpdateData(FALSE);
}

void SetVibrationActionDlg::Apply() {
	UpdateData(TRUE);
	GetAction()->SetVibration(m_vibration == 1);
}

BOOL SetVibrationActionDlg::Create(CWnd* wnd) {
	ASSERT_VALID(wnd);
	return CDialog::Create(IDD_ACTDLG_SET_VIBRATION,wnd);
}

SetVibrationAction* SetVibrationActionDlg::GetAction() {
	SetVibrationAction* action = dynamic_cast<SetVibrationAction*>(m_action);
	ASSERT_VALID(action);
	return action;
}
