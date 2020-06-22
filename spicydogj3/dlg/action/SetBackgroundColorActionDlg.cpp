#include "stdafx.h"
#include "resource.h"
#include "SetBackgroundColorActionDlg.h"
#include "model/action/SetBackgroundColorAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(SetBackgroundColorActionDlg,ActionDlg)
BEGIN_MESSAGE_MAP(SetBackgroundColorActionDlg, ActionDlg)
	ON_UPDATE_COMMAND_UI(IDCB_BACKGROUND_COLOR,OnBkgnd)
END_MESSAGE_MAP()

SetBackgroundColorActionDlg::SetBackgroundColorActionDlg() {
}


void SetBackgroundColorActionDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDCB_BACKGROUND_COLOR, m_btnColor);
}

void SetBackgroundColorActionDlg::SetAction(Appli* appli,Action* action) {
	ActionDlg::SetAction(appli,action);
	m_btnColor.SetColor(GetAction()->GetColor());
}

void SetBackgroundColorActionDlg::Apply() {
	UpdateData(TRUE);
	GetAction()->SetColor(m_btnColor.GetColor());
}

BOOL SetBackgroundColorActionDlg::Create(CWnd* wnd) {
	ASSERT_VALID(wnd);
	return CDialog::Create(IDD_ACTDLG_SET_BACKGROUND_COLOR,wnd);
}

SetBackgroundColorAction* SetBackgroundColorActionDlg::GetAction() {
	SetBackgroundColorAction* action = dynamic_cast<SetBackgroundColorAction*>(m_action);
	ASSERT_VALID(action);
	return action;
}

void SetBackgroundColorActionDlg::OnBkgnd(CCmdUI* pcmd) {
	pcmd->Enable(TRUE);
}
