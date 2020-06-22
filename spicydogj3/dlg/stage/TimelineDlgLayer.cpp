#include "stdafx.h"
#include "resource.h"
#include "TimelineDlg.h"
#include "TimelineDlgConf.H"
#include "cmd/stage/DelLayerCmd.h"
#include "SpicydogDoc.h"
#include "cmd/stage/InsLayerCmd.h"
#include "dlg/stage/RenLayerDlg.h"
#include "cmd/stage/RenLayerCmd.h"
#include "cmd/stage/MovLayerCmd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void TimelineDlg::OnDelLayer()  {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	if (!IsValidDelLayer())
		return;

	GetDocument()->DoCmd(new DelLayerCmd(GetTargetStage(),GetTargetStage()->GetSelLayer()));
	GetDocument()->UpdateAll(NULL);

	Invalidate();
}

void TimelineDlg::OnUpdateDelLayer(CCmdUI* pCmd) {
	pCmd->Enable(IsValidDelLayer());
}

BOOL TimelineDlg::IsValidDelLayer() const {
	if (!ExistActiveDocument())
		return FALSE;
	return GetTargetStage()->GetSelLayer()!= -1 && 1 < GetTargetStage()->GetLayerCount();
}

//	レイヤーを複製
void TimelineDlg::OnDupLayer() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();
	
	if (!IsValidDupLayer())
		return;

	Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);

	int nDup =stage->GetSelLayer();
	if (nDup == -1)
		return;

	const Layer* pSrcLayer = stage->GetLayer(nDup);
	ASSERT_VALID(pSrcLayer);

	Layer* layer = pSrcLayer->Clone();
	ASSERT_VALID(layer);
	layer->SetName(GetDocument()->layerNamingRule.DupIncNumName(pSrcLayer->GetName()));
	
	GetDocument()->DoCmd(new InsLayerCmd(GetTargetStage(),nDup+1,layer));	
	GetTargetStage()->SelectLayer(nDup);
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateDupLayer(CCmdUI* pCmd) {
	pCmd->Enable(IsValidDupLayer());
}

BOOL TimelineDlg::IsValidDupLayer() const {
	if (!ExistActiveDocument())
		return FALSE;

	return GetTargetStage()->GetSelLayer()!= -1;
}

//	レイヤーを挿入
void TimelineDlg::OnInsLayer() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();
	
	if (!IsValidInsLayer())
		return;

	Stage* stage = GetTargetStage();
	int nInsert =stage->GetSelLayer();
	if (nInsert < 0)
		nInsert = stage->GetLayerCount();

	Layer* layer = new Layer;	
	layer->SetName(GetDocument()->layerNamingRule.NewIncNumName(IDS_NEW_LAYER_NAME));
	ASSERT_VALID(layer);

	GetDocument()->DoCmd(new InsLayerCmd(GetTargetStage(),nInsert,layer));	
	GetTargetStage()->SelectLayer(nInsert);
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateInsLayer(CCmdUI* pCmd) {
	pCmd->Enable(IsValidInsLayer());
}

BOOL TimelineDlg::IsValidInsLayer() const {
	if (!ExistActiveDocument())
		return FALSE;
	return TRUE;
}

void TimelineDlg::OnRenLayer() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	if (!IsValidRenLayer())
		return;

	Layer* layer = GetTargetStage()->GetLayer(GetTargetStage()->GetSelLayer());
	RenLayerDlg dlg(layer->GetName(),this);
	if (dlg.DoModal() == IDOK) {
		GetDocument()->DoCmd(new RenLayerCmd(layer,dlg.GetName()));
		GetDocument()->UpdateAll(NULL);
	}
}

void TimelineDlg::OnUpdateRenLayer(CCmdUI* pCmd) {
	pCmd->Enable(IsValidRenLayer());
}

BOOL TimelineDlg::IsValidRenLayer() const {
	if (!ExistActiveDocument())
		return FALSE;
	return GetTargetStage()->GetSelLayer() != -1;
}

void TimelineDlg::OnOrderDown()  {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();
	if (!IsValidOrderDown()) {
		return;
	}
	int src = GetTargetStage()->GetSelLayer();
	int dst = src+1;
	GetDocument()->DoCmd(new MovLayerCmd(GetTargetStage(),dst,src));
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateOrderDown(CCmdUI* pCmd) {
	pCmd->Enable(IsValidOrderDown());
}

BOOL TimelineDlg::IsValidOrderDown() const {
	if (!ExistActiveDocument())
		return FALSE;

	int nCount    = GetTargetStage()->GetLayerCount();
	int nSelLayer = GetTargetStage()->GetSelLayer();
	return nSelLayer != -1 && 0 < nCount && nSelLayer < (nCount-1);
}

void TimelineDlg::OnOrderUp()  {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();
	if (!IsValidOrderUp()) {
		return;
	}
	int src = GetTargetStage()->GetSelLayer();
	int dst = src-1;
	GetDocument()->DoCmd(new MovLayerCmd(GetTargetStage(),dst,src));
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateOrderUp(CCmdUI* pCmd) {
	pCmd->Enable(IsValidOrderUp());
}

BOOL TimelineDlg::IsValidOrderUp() const {
	if (!ExistActiveDocument())
		return FALSE;

	int nCount    = GetTargetStage()->GetLayerCount();
	int nSelLayer = GetTargetStage()->GetSelLayer();
	return nSelLayer != -1 && 0 < nCount && 0 < nSelLayer;
}

//	全てのレイヤーを表示する
void TimelineDlg::OnShowAllLayers() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	if (!IsValidShowAllLayers())
		return;

	SpicydogDoc* pDoc = GetDocument();
	Stage* stage = GetTargetStage();
	stage->ShowAll();
	pDoc->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateShowAllLayers(CCmdUI* pCmd) {
	pCmd->Enable(IsValidShowAllLayers());
}

BOOL TimelineDlg::IsValidShowAllLayers() const {
	if (!ExistActiveDocument())
		return FALSE;

	return GetTargetStage()->GetSelLayer() != -1 && GetTargetStage()->GetLayerCount() != 1;
}

void TimelineDlg::OnLockOtherLayers() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	if (!IsValidLockOtherLayers())
		return;

	SpicydogDoc* pDoc = GetDocument();
	Stage* stage = GetTargetStage();;
	stage->LockOtherLayers(GetTargetStage()->GetSelLayer());
	pDoc->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateLockOtherLayers(CCmdUI* pCmd) {
	pCmd->Enable(IsValidLockOtherLayers());
}

BOOL TimelineDlg::IsValidLockOtherLayers() const {
	if (!ExistActiveDocument())
		return FALSE;

	return GetTargetStage()->GetSelLayer() != -1 && GetTargetStage()->GetLayerCount() != 1;
}

void TimelineDlg::OnHideOtherLayers() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();
	if (!IsValidHideOtherLayers())
		return;

	SpicydogDoc* pDoc = GetDocument();
	Stage* stage = GetTargetStage();
	stage->HideOtherLayers(stage->GetSelLayer());
	pDoc->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateHideOtherLayers(CCmdUI* pCmd) {
	pCmd->Enable(IsValidHideOtherLayers());
}

BOOL TimelineDlg::IsValidHideOtherLayers() const {
	if (!ExistActiveDocument())
		return FALSE;

	return GetTargetStage()->GetSelLayer() != -1 && GetTargetStage()->GetLayerCount() != 1;
}


