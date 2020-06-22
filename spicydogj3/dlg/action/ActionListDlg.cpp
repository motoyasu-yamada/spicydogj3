#include "stdafx.h"
#include "resource.h"
#include "ActionListDlg.h"

#include "ActionDlg.h"
#include "model/Appli.h"
#include "model/action/ActionFactory.h"
#include "model/action/ActionList.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(ActionListDlg, CDialog)
	ON_LBN_SELCHANGE(IDCL_ACTIONLIST, OnSelchangeAction)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCB_ACT_INS, OnActAdd)
	ON_BN_CLICKED(IDCB_ACT_DEL, OnActDel)
	ON_BN_CLICKED(IDCB_ACT_DOWN,OnActDown)
	ON_BN_CLICKED(IDCB_ACT_UP,  OnActUp)
	ON_COMMAND(ID_INSACT_PLAY,OnInsPlay)
	ON_COMMAND(ID_INSACT_STOP,OnInsStop)
	ON_COMMAND(ID_INSACT_JUMP,OnInsJump)
	ON_COMMAND(ID_INSACT_QUIT,OnInsQuit)
	ON_COMMAND(ID_INSACT_SOUND_PLAY,OnInsSoundPlay)
	ON_COMMAND(ID_INSACT_SOUND_STOP,OnInsSoundStop)
	ON_COMMAND(ID_INSACT_SET_BACKLIGHT,       OnInsSetBacklight)
	ON_COMMAND(ID_INSACT_SET_BACKGROUND_COLOR,OnInsSetBackgroundColor)
	ON_COMMAND(ID_INSACT_SET_VIBRATION,       OnInsSetVibration)
	ON_COMMAND(ID_INSACT_SET_SOFTKEY, OnInsSetSoftkey)
	ON_COMMAND(ID_INSACT_DELAY_JUMP , OnInsDelayJump)
	ON_COMMAND(ID_INSACT_RANDOM_JUMP, OnInsRandomJump)
END_MESSAGE_MAP()

ActionListDlg::ActionListDlg() {
	m_selectAction       = NULL;
	m_appli    = NULL;
}

ActionListDlg::~ActionListDlg() {
}

const ActionList& ActionListDlg::GetEditActionList() const {
	return m_editActionList;
}

BOOL ActionListDlg::Create(Appli* appli,const ActionList* actionList,CWnd* pParent,LPCRECT rect) {
	ASSERT_VALID(actionList);
	ASSERT_VALID(appli);
	ASSERT(AfxIsValidAddress(rect,sizeof RECT));
	if (!CDialog::Create(IDD_ACTION,pParent)) {
		return FALSE;
	}
	m_appli              = appli;
	m_editActionList.CopyFrom(actionList);
	int width  = rect->right  - rect->left;
	int height = rect->bottom - rect->top;
	SetWindowPos(NULL,rect->left,rect->top,width,height,SWP_NOACTIVATE | SWP_NOZORDER);
	ShowWindow(SW_SHOW);
	UpdateWindow();
	UpdateActionList();
	ASSERT_VALID(this);
	return TRUE;
}

void ActionListDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCS_ACTIONNAME, m_wndActionName);
	DDX_Control(pDX, IDCS_NOPROP, m_stNoProp);
	DDX_Control(pDX, IDCS_PROPDLG, m_stPropDlg);
	DDX_Control(pDX, IDCL_ACTIONLIST, m_lbActions);
}


void ActionListDlg::UpdateCtrls() {
	EnableDlgItem(this,IDCB_ACT_DEL, IsSelected());
	EnableDlgItem(this,IDCB_ACT_DOWN,IsValidActDown());
	EnableDlgItem(this,IDCB_ACT_UP,  IsValidActUp());
}


BOOL ActionListDlg::IsValidActUp() const
{
	int nSel = m_lbActions.GetCurSel();
	return nSel != -1 && 0 < nSel;
}

BOOL ActionListDlg::IsValidActDown() const
{
	int nSel = m_lbActions.GetCurSel();
	return nSel != -1 && nSel < (m_lbActions.GetCount()-1);
}

BOOL ActionListDlg::IsSelected() const
{
	return m_lbActions.GetCurSel() != -1;
}

void ActionListDlg::UpdateCurrentData() {
	if (m_dialog.get() != NULL) {
		ASSERT_VALID(m_dialog.get());
		m_dialog->UpdateData(TRUE);
		m_dialog->Apply();
	}
}

void ActionListDlg::HidePrevPropDlg() {
	if (m_dialog.get() != NULL) {
		ASSERT_VALID(m_dialog.get());
		m_dialog->UpdateData(TRUE);
		m_dialog->Apply();
		m_dialog->DestroyWindow();
		m_dialog       = DIALOGPTR(NULL);
	}
	m_selectAction = NULL;
}

void ActionListDlg::DisplayPropDlg() {
	m_stNoProp.ShowWindow(SW_HIDE);

	CRuntimeClass* dialogClass = m_selectAction->GetDialogClass();
	ASSERT(dialogClass != NULL);
	CObject* object = dialogClass->CreateObject();
	if (object == NULL) {
		AfxThrowResourceException();
	}
	ActionDlg* dialog = DYNAMIC_DOWNCAST(ActionDlg,object);
	if (dialog == NULL) {
		AfxThrowResourceException();
	}
	if (!dialog->Create(this)) {
		AfxThrowResourceException();
	}

	CRect rc;
	m_stPropDlg.GetWindowRect(rc);
	ScreenToClient(rc);

	dialog->SetAction(m_appli,m_selectAction);
	dialog->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	dialog->ShowWindow(SW_SHOW);
	dialog->UpdateWindow();
	m_dialog = DIALOGPTR(dialog);
}

void ActionListDlg::DisplayNonePropDlg() {
	m_stNoProp.ShowWindow(SW_SHOW);
	ASSERT(m_dialog.get() == NULL);
}

void ActionListDlg::OnSelchangeAction() {
	Action* newAction = NULL;
	int select = m_lbActions.GetCurSel();
	if (select != -1) {
		newAction = m_editActionList.GetAction(select);
		ASSERT_VALID(newAction);
	}

	if (m_selectAction != newAction) {
		if (m_selectAction != NULL) {
			HidePrevPropDlg();
		}
		if (select == -1) {
			m_stNoProp.ShowWindow(SW_SHOW);
			m_wndActionName.SetWindowText(NULL);

		} else {
			m_wndActionName.SetWindowText(newAction->GetActionName());
			m_selectAction = newAction;
			if (m_selectAction->GetDialogClass() == NULL) {
				DisplayNonePropDlg();
			} else {
				DisplayPropDlg();
			}
		}
	}
	UpdateCtrls();
}

void ActionListDlg::OnActAdd() {
	TrackDlgBtnPopupMenu(this,IDCB_ACT_INS,IDM_ACTION);
	UpdateCtrls();
}

void ActionListDlg::OnActDel() {
	if (!IsSelected()) {
		return;
	}

	HidePrevPropDlg();
	int pos = m_lbActions.GetCurSel();
	m_editActionList.DelActCmd(pos);
	UpdateActionList();
	UpdateCtrls();
}

void ActionListDlg::OnActDown() {
	if (!IsValidActDown()) {
		return;
	}

	int nSel = m_lbActions.GetCurSel();
	m_lbActions.SetCurSel(nSel+1);
	ChangeActionOrder(nSel+1,nSel);
	UpdateCtrls();
}

void ActionListDlg::OnActUp() {
	if (!IsValidActUp())
		return;

	int nSel = m_lbActions.GetCurSel();
	m_lbActions.SetCurSel(nSel-1);
	ChangeActionOrder(nSel-1,nSel);
	UpdateCtrls();
}

void ActionListDlg::ChangeActionOrder(int n1,int n2) {
	if (n1 == n2) {
		return;
	}
	m_editActionList.MovActCmd(n1,n2);
	UpdateActionList();
}

void ActionListDlg::OnInsPlay() {
	InsAction(ActionFactory::NewPlayAction());
}

void ActionListDlg::OnInsStop() {
	InsAction(ActionFactory::NewStopAction());
}

void ActionListDlg::OnInsSoundPlay() {
	InsAction(ActionFactory::NewSoundPlayAction());
}

void ActionListDlg::OnInsSoundStop() {
	InsAction(ActionFactory::NewSoundStopAction());
}

void ActionListDlg::OnInsJump() {
	InsAction(ActionFactory::NewJumpAction());
}

void ActionListDlg::OnInsQuit() {
	InsAction(ActionFactory::NewQuitAction());
}

void ActionListDlg::OnInsSetBacklight() {
	InsAction(ActionFactory::NewSetBacklightAction());
}

void ActionListDlg::OnInsSetBackgroundColor() {
	InsAction(ActionFactory::NewSetBackgroundColorAction());
}

void ActionListDlg::OnInsSetVibration() {
	InsAction(ActionFactory::NewSetVibrationAction());
}

void ActionListDlg::OnInsSetSoftkey() {
	InsAction(ActionFactory::NewSetSoftkeyAction());
}

void ActionListDlg::OnInsDelayJump() {
	InsAction(ActionFactory::NewDelayJumpAction());
}

void ActionListDlg::OnInsRandomJump() {
	InsAction(ActionFactory::NewRandomJumpAction());
}

void ActionListDlg::InsAction(Action* action) {
	ASSERT_VALID(action);

	int select = m_lbActions.GetCurSel();
	if (select == -1) {
		m_editActionList.InsActCmd(m_editActionList.GetActionCount(),action);
		select = m_lbActions.GetCount();
		UpdateActionList();
		m_lbActions.SetCurSel(select);
		OnSelchangeAction();
	} else {
		m_editActionList.InsActCmd(select+1,action);
		UpdateActionList();
	}
	UpdateCtrls();
}

void ActionListDlg::UpdateActionList() {
	int select = m_lbActions.GetCurSel();

	m_lbActions.SetRedraw(FALSE);
		while (0 < m_lbActions.GetCount()) {
			m_lbActions.DeleteString(0);
		}
		for (int n = 0;n < m_editActionList.GetActionCount();n++) {
			Action* action =	m_editActionList.GetAction(n);
			m_lbActions.AddString(action->GetActionName());
		}
		if (select != -1 && select < m_editActionList.GetActionCount()) {
			m_lbActions.SetCurSel(select);
		}
	m_lbActions.SetRedraw(TRUE);

	OnSelchangeAction();
}

BOOL ActionListDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	UpdateActionList();
	UpdateCtrls();

	return TRUE;
}

void ActionListDlg::OnDestroy() {
	HidePrevPropDlg();	
	CDialog::OnDestroy();	
}

void ActionListDlg::OnCancel() {
}

void ActionListDlg::OnOK() {
}
