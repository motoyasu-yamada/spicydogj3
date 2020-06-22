#include "stdafx.h"
#include "resource.h"
#include "StagesDlg.h"

#include "cmd/stage/InsSubStageCmd.h"
#include "cmd/stage/DelSubStageCmd.h"
#include "cmd/stage/RenSubStageCmd.h"
#include "dlg/stage/RenSubStageDlg.h"
#include "model/Appli.h"
#include "model/stage/MainStage.h"
#include "model/stage/Stage.h"
#include "model/stage/SubStage.h"
#include "model/stage/SubStages.h"
#include "SpicydogDoc.h"
#include "utility/WndUtility.h"
#include "utility/StringUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int DEFAULT_NAMECOL_WIDTH = 128;
const int DEFAULT_SIZECOL_WIDTH  = 64;
const int DEFAULT_LAYERCOL_WIDTH = 48;
const int DEFAULT_FRAMECOL_WIDTH = 48;

const int COL_NAME  = 0;
const int COL_SIZE  = 1;
const int COL_LAYER = 2;
const int COL_FRAME = 3;

BEGIN_MESSAGE_MAP(StagesDlg, DialogBarDlg)
	ON_NOTIFY(NM_RCLICK, IDCL_STAGES, OnRclickStages)
	ON_NOTIFY(NM_DBLCLK, IDCL_STAGES, OnDblclkStages)
	ON_WM_SIZE()
	ON_COMMAND(ID_SUBSTAGES_DEL,OnDelSubStage)
	ON_COMMAND(ID_SUBSTAGES_INS,OnInsSubStage)
	ON_COMMAND(ID_SUBSTAGES_REN,OnRenSubStage)
	ON_COMMAND(ID_STAGES_EDIT,OnEdit)
	ON_UPDATE_COMMAND_UI(ID_STAGES_EDIT,  OnUpdate_ReqSel)
	ON_UPDATE_COMMAND_UI(ID_SUBSTAGES_DEL,OnUpdate_ReqSelSub)
	ON_UPDATE_COMMAND_UI(ID_SUBSTAGES_INS,OnUpdate_ReqActive)
	ON_UPDATE_COMMAND_UI(ID_SUBSTAGES_REN,OnUpdate_ReqSelSub)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(StagesDlg,DialogBarDlg)

StagesDlg::StagesDlg()
:	DialogBarDlg() {
	m_bInited        = FALSE;
	m_activeStage    = NULL;
	m_activeDocument = NULL;
	VERIFY(m_images.Create(MAKEINTRESOURCE(IDB_STAGES),16,1,RGB(0x00,0x80,0x80)));
}

StagesDlg::~StagesDlg(){
}

void StagesDlg::DoDataExchange(CDataExchange* pDX) {
	DialogBarDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDCL_STAGES, m_list);
}

void StagesDlg::OnUpdate_ReqActive(CCmdUI* pCmd) {
	pCmd->Enable(ExistActiveDocument());
}

void StagesDlg::OnUpdate_ReqSel(CCmdUI* pCmd) {
	pCmd->Enable(ExistActiveDocument() && GetListCtrlCurSel(m_list) != -1);
}

void StagesDlg::OnUpdate_ReqSelSub(CCmdUI* pCmd) {
	pCmd->Enable(ExistActiveDocument() && 0 < GetListCtrlCurSel(m_list));
}

Stage* StagesDlg::GetSelStage() {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return NULL;
	}
	const int nSel = GetListCtrlCurSel(m_list);
	if (nSel == -1) {
		return NULL;
	}
	if (nSel == 0) {
		return m_activeDocument->appli->mainStage;
	}
	SubStages* subStages = m_activeDocument->appli->subStages.get();
	const int substageIndex = nSel -1;
	ASSERT(0 <= substageIndex);
	if (subStages->GetSubStageCount() <= substageIndex) {
		ASSERT(FALSE);
		return NULL;
	}
	return subStages->GetSubStage(substageIndex);
}

void StagesDlg::OnDocumentActivated(CDocument* p) {
	DialogBarDlg::OnDocumentActivated(p);

	BOOL bActive = FALSE;
	if (ExistActiveDocument()) {
		m_activeDocument = DYNAMIC_DOWNCAST(SpicydogDoc,p);
		ASSERT_VALID(m_activeDocument);
		bActive = TRUE;
	} else {
		m_list.DeleteAllItems();
		m_activeDocument = NULL;
	}
	m_list.EnableWindow(bActive);
	m_toolbar.EnableWindow(bActive);
}

CString itos(int n) {
	return FormatString("%d",n);
}

void StagesDlg::OnNotify(DWORD dwNotifyCode,CDocument* p,LPARAM,LPVOID) {
	if (dwNotifyCode != SpicydogDoc::EVENTNOTIFYCODE_BUILT)
		return;
	
	OnDocumentUpdated(p,0,NULL);
}

void StagesDlg::OnDocumentUpdated(CDocument* p,LPARAM lHint,CObject*) {
	if (lHint == SpicydogDoc::UPDATED_PLAY)
		return;
	if (!ExistActiveDocument()) {
		return;
	}
	ASSERT_VALID(m_activeDocument);

	int nSel = GetListCtrlCurSel(m_list);

	m_list.DeleteAllItems();
	const MainStage* mainStage = m_activeDocument->appli->mainStage;
	m_list.InsertItem(0,NULL);
		m_list.SetItem(0,COL_NAME, LVIF_TEXT | LVIF_IMAGE,mainStage->GetStageName(),0,0,0,0);
		m_list.SetItem(0,COL_SIZE, LVIF_TEXT,itos(mainStage->GetBuiltSize()),0,0,0,0);
		m_list.SetItem(0,COL_LAYER,LVIF_TEXT,itos(mainStage->GetLayerCount()),0,0,0,0);
		m_list.SetItem(0,COL_FRAME,LVIF_TEXT,itos(mainStage->GetFrameCount()),0,0,0,0);

	const SubStages* stages = m_activeDocument->appli->subStages.get();
	ASSERT_VALID(stages);
	const int num = stages->GetSubStageCount();
	for (int n = 0;n < num;n++) {
		const SubStage* p = stages->GetSubStage(n);
		const int row = n + 1;
		m_list.InsertItem(row,NULL);
			m_list.SetItem(row,COL_NAME, LVIF_TEXT | LVIF_IMAGE,p->GetStageName(),1,0,0,0);
			m_list.SetItem(row,COL_SIZE, LVIF_TEXT,itos(p->GetBuiltSize()),  0,0,0,0);
			m_list.SetItem(row,COL_LAYER,LVIF_TEXT,itos(p->GetLayerCount()), 0,0,0,0);
			m_list.SetItem(row,COL_FRAME,LVIF_TEXT,itos(p->GetFrameCount()), 0,0,0,0);
	}
	if (nSel != -1) {
		m_list.SetItemState(nSel,LVIS_SELECTED ,LVIS_SELECTED);
		m_list.EnsureVisible(nSel,FALSE);
	}
}

void StagesDlg::OnRclickStages(NMHDR* pNMHDR, LRESULT* pResult) {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}

	m_activeDocument->BeginEdit();
	TrackNewPopupMenu(GetTopLevelFrame(),IDM_STAGES);
	*pResult = 0;
}

void StagesDlg::OnDblclkStages(NMHDR* pNMHDR, LRESULT* pResult)  {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}

	m_activeDocument->BeginEdit();
	Stage* stage = GetSelStage();
	if (stage != NULL) {
		m_activeDocument->appli->ActivateStage(stage);
		m_activeDocument->UpdateAll(NULL);
	}
}

void StagesDlg::OnEdit() {
	ActivateSelStage();
}

void StagesDlg::ActivateSelStage() {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}

	Stage* stage = GetSelStage();
	if (stage == NULL) {
		ASSERT(FALSE);
		return;
	}
	m_activeDocument->BeginEdit();
	m_activeDocument->appli->ActivateStage(stage);
	m_activeDocument->UpdateAll(NULL);
}

void StagesDlg::OnInsSubStage() {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}
	m_activeDocument->BeginEdit();

	SubStages* subStages = m_activeDocument->appli->subStages.get();
	ASSERT_VALID(subStages);
	SubStage* stage = new SubStage;
	CString str = m_activeDocument->stageNamingRule.NewIncNumName(IDS_NEW_STAGE_NAME);
	stage->SetStageName(str);
	InsSubStageCmd* p = new InsSubStageCmd(subStages,subStages->GetSubStageCount(),stage);
	m_activeDocument->DoCmd(p);
	m_activeDocument->UpdateAll(NULL);
}

void StagesDlg::OnDelSubStage() {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}
	m_activeDocument->BeginEdit();

	Stage* stage = GetSelStage();
	if (stage == NULL ||
		stage == (Stage*)m_activeDocument->appli->mainStage) {
		return;
	}
	SubStages* subStages = m_activeDocument->appli->subStages.get();
	if (m_activeDocument->appli->GetActiveStage() == stage) {
		m_activeDocument->appli->ActivateStage(m_activeDocument->appli->mainStage);
	}

	int nDel = subStages->GetSubStageIndex((SubStage*)stage);
	if (nDel == -1)
		{ASSERT(FALSE);return;}

	DelSubStageCmd* cmd = new DelSubStageCmd(subStages,nDel);
	m_activeDocument->DoCmd(cmd);
	m_activeDocument->UpdateAll(NULL);
}

void StagesDlg::OnRenSubStage() {
	if (m_activeDocument == NULL) {
		ASSERT(FALSE);
		return;
	}

	Stage* stage = GetSelStage();
	if (stage == NULL ||
		stage == (Stage*)m_activeDocument->appli->mainStage) {
		return;
	}

	SubStage* subStage = dynamic_cast<SubStage*>(stage);
	ASSERT_VALID(subStage);

	RenSubStageDlg dlg(this);
	dlg.m_strName = subStage->GetStageName();
	if (dlg.DoModal() == IDOK) {
		RenSubStageCmd* cmd = new RenSubStageCmd(subStage,dlg.m_strName);
		m_activeDocument->DoCmd(cmd);
		m_activeDocument->UpdateAll(NULL);
	}
}

void StagesDlg::OnSize(UINT nType, int cx, int cy)  {
	DialogBarDlg::OnSize(nType, cx, cy);
	if (!m_bInited) {
		return;
	}
	MoveChildWindowToBottom(m_toolbar,cy);
	StretchChildWindowToRight(m_list,cx);
	StretchChildWindowToBottom(m_list,cy - GetWindowHeight(m_toolbar));
}

void InsertStageColumn(CListCtrl& list,int col,UINT name,int fmt,int width) {
	if (col != list.InsertColumn(col,LoadString(name),fmt,width,col)) {
		ASSERT(FALSE);
		AfxThrowResourceException();
	}
}

BOOL StagesDlg::OnInitDialog() {
	DialogBarDlg::OnInitDialog();
	if (!m_toolbar.Create(IDT_STAGES,this,-1,WS_CHILD|WS_VISIBLE,1)) {
		ASSERT(FALSE);
		AfxThrowResourceException();
	}

	InsertStageColumn(m_list,COL_NAME,IDS_STAGESDLG_NAME,  LVCFMT_LEFT,DEFAULT_NAMECOL_WIDTH);
	InsertStageColumn(m_list,COL_SIZE,IDS_STAGESDLG_SIZE,  LVCFMT_LEFT,DEFAULT_SIZECOL_WIDTH); 
	InsertStageColumn(m_list,COL_LAYER,IDS_STAGESDLG_LAYER,LVCFMT_LEFT,DEFAULT_LAYERCOL_WIDTH); 
	InsertStageColumn(m_list,COL_FRAME,IDS_STAGESDLG_FRAME,LVCFMT_LEFT,DEFAULT_FRAMECOL_WIDTH); 
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_list.SetImageList(&m_images,LVSIL_SMALL);

	m_bInited = TRUE;
	return TRUE;
}
