#include "stdafx.h"
#include "resource.h"
#include "RandomJumpActionDlg.h"

#include "model/action/RandomJumpAction.h"
#include "RandomJumpCaseDlg.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(RandomJumpActionDlg,ActionDlg)

BEGIN_MESSAGE_MAP(RandomJumpActionDlg, ActionDlg)
	ON_NOTIFY(NM_CLICK, IDCL_CASES, OnClickCases)
	ON_BN_CLICKED(IDCB_DEL, OnDel)
	ON_BN_CLICKED(IDCB_INS, OnIns)
	ON_BN_CLICKED(IDCB_MOD, OnMod)
	ON_UPDATE_COMMAND_UI(IDCB_DEL,OnUpdateDel)
	ON_UPDATE_COMMAND_UI(IDCB_INS,OnUpdateIns)
	ON_UPDATE_COMMAND_UI(IDCB_MOD,OnUpdateMod)
END_MESSAGE_MAP()

RandomJumpActionDlg::RandomJumpActionDlg()
: ActionDlg() {
}

void RandomJumpActionDlg::DoDataExchange(CDataExchange* pDX) {
	ActionDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDCL_CASES, m_listCases);
}


BOOL RandomJumpActionDlg::Create(CWnd* wnd) {
	return CDialog::Create(IDD_ACTDLG_RANDOM_JUMP,wnd);
}

void RandomJumpActionDlg::SetAction(Appli* appli,Action* action) {
	ActionDlg::SetAction(appli,action);

	UpdateData(FALSE);
	UpdateList();
}

void RandomJumpActionDlg::Apply() {
	ASSERT_VALID(this);
}

RandomJumpAction* RandomJumpActionDlg::GetAction() {
	RandomJumpAction* action = dynamic_cast<RandomJumpAction*>(m_action);
	ASSERT_VALID(action);
	return action;
}

int GetCurSingleSel(const CListCtrl& ctr) {
	int n = ctr.GetSelectedCount();
	if (n == 0) {
		return LB_ERR;
	}
	ASSERT(n == 1);

	POSITION pos = ctr.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		return ctr.GetNextSelectedItem(pos);
	} else {
		return LB_ERR;
	}
}

void RandomJumpActionDlg::OnClickCases(NMHDR* pNMHDR, LRESULT* pResult) {
	UpdateCtrls();	
	*pResult = 0;
}

void RandomJumpActionDlg::OnDel() {
	int n = GetCurSingleSel(m_listCases);
	if (n == LB_ERR) {
		ASSERT(FALSE);
		return;
	}
	GetAction()->DeleteCase(n);
	UpdateList();
}

void RandomJumpActionDlg::OnIns() {
	RandomJumpCase rcase;
	rcase.SetCase(1,NULL);
	
	RandomJumpCaseDlg dlg(m_appli,&rcase,this);
	if (dlg.DoModal() == IDOK) {
		GetAction()->AppendCase(rcase);
		UpdateList();
	}
}

void RandomJumpActionDlg::OnMod() {
	int n = GetCurSingleSel(m_listCases);
	if (n == LB_ERR) {
		ASSERT(FALSE);
		return;
	}

	RandomJumpCase rcase;
	GetAction()->GetCase(n,rcase);
	RandomJumpCaseDlg dlg(m_appli,&rcase,this);
	if (dlg.DoModal() == IDOK) {
		GetAction()->SetCase(n,rcase);
		UpdateList();
	}
}

void RandomJumpActionDlg::UpdateCtrls() {
	RandomJumpAction* p = GetAction();
	EnableDlgItem(this,IDCB_INS,p->GetCaseCount() < RandomJumpAction::MAX_CASE_COUNT);
	BOOL bSel = GetCurSingleSel(m_listCases) != LB_ERR;
	EnableDlgItem(this,IDCB_MOD,bSel);
	EnableDlgItem(this,IDCB_DEL,bSel && 1 < p->GetCaseCount());
}

void RandomJumpActionDlg::OnUpdateDel(CCmdUI* p) {
	p->Enable(GetCurSingleSel(m_listCases) != LB_ERR &&
		      1 < GetAction()->GetCaseCount());
}

void RandomJumpActionDlg::OnUpdateIns(CCmdUI* p) {
	p->Enable(GetAction()->GetCaseCount() < RandomJumpAction::MAX_CASE_COUNT);
}

void RandomJumpActionDlg::OnUpdateMod(CCmdUI* p) {
	p->Enable(GetCurSingleSel(m_listCases) != LB_ERR);
}

void RandomJumpActionDlg::UpdateList() {
	m_listCases.SetRedraw(FALSE);

		int select = GetCurSingleSel(m_listCases);
		m_listCases.DeleteAllItems();

		RandomJumpAction* action = GetAction();
		ASSERT_VALID(action);

		int nt = action->GetTotalCase();
		int nc = action->GetCaseCount();
		for (int n = 0;n < nc;n++) {
			RandomJumpCase rcase;
			CString strL;
			CString strR;
			int probability;

			GetAction()->GetCase(n,rcase);
			probability = rcase.GetProbability();

			m_listCases.InsertItem(n,NULL);
				
			if (rcase.GetLabel() == NULL) {
				strL = _T("-");
			} else {
				strL = rcase.GetLabel()->GetLabelName();
			}

			m_listCases.SetItem(n,1,LVIF_TEXT,strL,(select != n) ? 0 : LVIS_SELECTED,LVIS_SELECTED,0,0);

			if (nt == 0) {
				strR.Format(_T("%d"),probability);
			} else { 
				strR.Format(_T("%d(%d%%)"),probability,(probability * 100 / nt));
			}
			m_listCases.SetItem(n,0,LVIF_TEXT,strR,0,0,0,0);
		}

	m_listCases.SetRedraw(TRUE);

	UpdateCtrls();
}

BOOL RandomJumpActionDlg::OnInitDialog()  {
	CString string;

	ActionDlg::OnInitDialog();

	m_listCases.SetExtendedStyle(m_listCases.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	VERIFY(string.LoadString(IDS_RANDOM_JUMP_COL_LABEL));
	m_listCases.InsertColumn(1,string,LVCFMT_LEFT,128,0);

	VERIFY(string.LoadString(IDS_RANDOM_JUMP_COL_PROBABILITY));
	m_listCases.InsertColumn(0,string,LVCFMT_LEFT,    96,1);
	
	return TRUE;
}
