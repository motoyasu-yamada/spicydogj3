#include "stdafx.h"
#include "resource.h"
#include "KeyFrameActionDlg.h"
#include "model/Appli.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(KeyFrameActionDlg, CDialog)
END_MESSAGE_MAP()

KeyFrameActionDlg::KeyFrameActionDlg(Appli* appli,const ActionList* actionList,CWnd* pParent)
:	CDialog(IDD_KEY_FRAME_ACTION, pParent) {
	m_actionList = actionList;
	m_appli = appli;
	ASSERT_VALID(m_appli);
	ASSERT_VALID(m_actionList);
}

void KeyFrameActionDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	if (pDX->m_bSaveAndValidate) {
		m_dlgActionList.UpdateCurrentData();
	}
}

BOOL KeyFrameActionDlg::OnInitDialog() {
	CWnd* wnd = GetDlgItem(IDCS_ACTIONLIST);
	ASSERT_VALID(wnd);
	CRect rc;
	wnd->GetWindowRect(rc);
	ScreenToClient(rc);
	if (m_dlgActionList.GetSafeHwnd() != NULL) {
		m_dlgActionList.DestroyWindow();
	}
	return m_dlgActionList.Create(m_appli,m_actionList,this,rc);
}

const ActionList* KeyFrameActionDlg::GetEditActionList() const {
	ASSERT_VALID(&m_dlgActionList);
	return &m_dlgActionList.GetEditActionList();
}
