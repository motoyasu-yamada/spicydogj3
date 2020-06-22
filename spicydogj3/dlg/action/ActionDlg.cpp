#include "stdafx.h"
#include "ActionDlg.h"

#include "model/action/Action.h"
#include "model/Appli.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(ActionDlg, CDialog)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(ActionDlg,CDialog)
ActionDlg::ActionDlg() {
	m_action = NULL;
	m_appli  = NULL;
}

ActionDlg::~ActionDlg() {
}

void ActionDlg::SetAction(Appli* appli,Action* action) {
	ASSERT(m_action == NULL);
	ASSERT(AfxIsValidAddress(action,sizeof Action));
	ASSERT(AfxIsValidAddress(appli, sizeof Appli));

	m_action = action;
	m_appli  = appli;
}

BOOL ActionDlg::Create(CWnd* wnd) {
	ASSERT_VALID(wnd);
	ASSERT(FALSE);
	return FALSE;
}

void ActionDlg::Apply() {
	ASSERT(FALSE);
}

BOOL ActionDlg::IsInited() const {
	return m_action != NULL;
}

void ActionDlg::OnOK() {
	ASSERT(FALSE);
}

void ActionDlg::OnCancel() {
	ASSERT(FALSE);
}
