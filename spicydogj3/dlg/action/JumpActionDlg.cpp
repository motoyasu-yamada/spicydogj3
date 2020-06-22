#include "stdafx.h"
#include "resource.h"
#include "JumpActionDlg.h"

#include "model/Appli.h"
#include "model/stage/Label.h"
#include "model/action/JumpAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(JumpActionDlg, ActionDlg)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(JumpActionDlg,ActionDlg)
JumpActionDlg::JumpActionDlg()
:	ActionDlg()
{
}

void JumpActionDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCC_LABEL_LIST, m_cbLabelList);	
}

void JumpActionDlg::SetAction(Appli* appli,Action* action) {
	ActionDlg::SetAction(appli,action);
	appli->GetLabelList(m_labels);
	UpdateLabelList();
}

void JumpActionDlg::Apply() {
	int select = m_cbLabelList.GetCurSel();
	if (select == -1) {
		GetAction()->SetLabel(NULL);
	} else {
		LABEL_NAME_LIST::const_iterator i;
		i = m_labels.begin();
		std::advance(i,select);
		GetAction()->SetLabel(i->first);
	}
}

BOOL JumpActionDlg::Create(CWnd* wnd) {
	ASSERT_VALID(wnd);
	return CDialog::Create(IDD_ACTDLG_JUMP,wnd);
}

JumpAction* JumpActionDlg::GetAction() {
	JumpAction* action = dynamic_cast<JumpAction*>(m_action);
	ASSERT_VALID(action);
	return action;
}

void JumpActionDlg::UpdateLabelList() {
	Label::SetupLabelComboBox(m_cbLabelList,m_labels,GetAction()->GetLabel());
}

