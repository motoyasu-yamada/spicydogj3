#include "stdafx.h"
#include "resource.h"
#include "DelayJumpActionDlg.h"

#include "model/Appli.h"
#include "model/action/DelayJumpAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(DelayJumpActionDlg, CDialog)
	//{{AFX_MSG_MAP(DelayJumpActionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(DelayJumpActionDlg,ActionDlg)
DelayJumpActionDlg::DelayJumpActionDlg()
:	ActionDlg()
{
	m_delay = 0;
}

void DelayJumpActionDlg::DoDataExchange(CDataExchange* pDX) {
	DDX_Control(pDX, IDCC_LABEL_LIST,m_combo);
	DDX_Text   (pDX, IDCE_DELAY,     m_delay);
}

BOOL DelayJumpActionDlg::Create(CWnd* pParent) {
	return CDialog::Create(IDD,pParent);
}

void DelayJumpActionDlg::SetAction(Appli* appli,Action* _action) {
	ActionDlg::SetAction(appli,_action);
	DelayJumpAction* action = (DelayJumpAction*)_action;

	appli->GetLabelList(m_labels);
	Label::SetupLabelComboBox(m_combo,m_labels,action->GetLabel());
	m_delay = action->GetDelay();

	UpdateData(FALSE);
}

void DelayJumpActionDlg::Apply() {
	VERIFY(UpdateData(TRUE));

	int select = m_combo.GetCurSel();
	const Label* label = NULL;

	if (select != -1) {
		LABEL_NAME_LIST::iterator i = m_labels.begin();
		std::advance(i,select);
		label = i->first;
	}

	GetAction()->SetLabel(m_delay,label);
}

DelayJumpAction* DelayJumpActionDlg::GetAction() {
	DelayJumpAction* action = dynamic_cast<DelayJumpAction*>(m_action);
	ASSERT_VALID(action);
	return action;
}
