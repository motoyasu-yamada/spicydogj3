#include "stdafx.h"
#include "resource.h"
#include "TimelineDlg.h"
#include "TimelineDlgConf.h"

#include "cmd/ModActionListCmd.h"
#include "cmd/stage/DelFramesCmd.h"
#include "cmd/stage/DelKeyFrameLabelCmd.h"
#include "cmd/stage/InsFramesCmd.h"
#include "cmd/stage/InsKeyFrameCmd.h"
#include "cmd/stage/InsKeyFrameLabelCmd.h"
#include "cmd/stage/ModKeyFrameCmd.h"
#include "cmd/stage/ModKeyFrameLabelCmd.h"
#include "dlg/stage/EditAnimatingDlg.h"
#include "dlg/stage/KeyFrameActionDlg.h"
#include "dlg/stage/KeyFrameLabelDlg.h"
#include "model/char/Char.h"
#include "model/stage/KeyFrame.h"
#include "model/stage/Layer.h"
#include "model/stage/Stage.h"
#include "Player.h"
#include "SpicydogDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void TimelineDlg::OnTweenProperty() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	KeyFrame* frame = NULL;
	if (!IsValidTweenProperty(frame) && frame == NULL) {
		return;
	}

	const Layer* layer = GetTargetStage()->GetLayer(GetTargetStage()->GetSelLayer());
	const Char* pChar = layer->GetAttachedChar();
	BOOL bAllowSizing = pChar != NULL && pChar->GetCharType().canModifyPlacing;
	KeyFrameState state;
	frame->GetState(&state);

	EditAnimatingDlg dlg(bAllowSizing,&state.animating,this);
	if (dlg.DoModal() == IDOK) {
		state.animating = dlg.editAnimating;	
		GetDocument()->DoCmd(new ModKeyFrameCmd(frame,&state));
		GetDocument()->UpdateAll(NULL);
	}
}

void TimelineDlg::OnUpdateTweenProperty(CCmdUI* pCmd) {
	KeyFrame* frame = NULL;
	pCmd->Enable(IsValidTweenProperty(frame));
}

BOOL TimelineDlg::IsValidTweenProperty(KeyFrame const*& key) {
	if (!ExistActiveDocument()) {
		return FALSE;
	}
	BOOL bEnabled = GetTargetStage()->IsFrameSel();
	if (bEnabled) {
		Layer* layer = GetTargetStage()->GetLayer(GetTargetStage()->GetSelLayer());

		key = layer->GetKeyFrame(GetTargetStage()->GetSelFrame());
		if (key == NULL)
			bEnabled = FALSE;
	}

	return bEnabled;
}

void TimelineDlg::OnUpdateActionsProp(CCmdUI* pCmdUI)  {
	KeyFrame* dummy;
	pCmdUI->Enable(IsValidActionsProp(dummy));
}

void TimelineDlg::OnActionsProp()  {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	KeyFrame* key = NULL;
	if (!IsValidActionsProp(key))
		return;

	KeyFrameActionDlg dlg(GetDocument()->appli.get(),key->GetActionList(),this);
	if (dlg.DoModal() == IDOK) {
		GetDocument()->DoCmd(new ModActionListCmd(key->GetActionList(),dlg.GetEditActionList()));
		GetDocument()->UpdateAll(NULL);
	}
}

BOOL TimelineDlg::IsValidActionsProp(KeyFrame const*& infoFrame) {
	return IsValidTweenProperty(infoFrame);
}

//	モーショントゥイーンを作成
void TimelineDlg::OnTweenCreate() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	KeyFrame* key = NULL;
	if (!IsValidTweenCreate(key))
		return;

	KeyFrameState state;
	key->GetState(&state);
	state.animating.animating = TRUE;
	GetDocument()->DoCmd(new ModKeyFrameCmd(key,&state));
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateTweenCreate(CCmdUI* pCmd) {
	KeyFrame* dummy;
	pCmd->Enable(IsValidTweenCreate(dummy));
}

BOOL TimelineDlg::IsValidTweenCreate(KeyFrame const*& infoFrame) {
	if (!ExistActiveDocument())
		return FALSE;

	return IsValidTweenProperty(infoFrame);
}	

void TimelineDlg::OnInsFrame() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	if (!IsValidInsFrame())
		return;

	Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	Layer* layer = stage->GetLayer(stage->GetSelLayer());
	int nStart;
	int nEnd;
	stage->GetSel(nStart,nEnd);

	GetDocument()->DoCmd(new InsFramesCmd(layer,nStart,nEnd - nStart + 1));
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateInsFrame(CCmdUI* pCmd) {
	pCmd->Enable(IsValidInsFrame());
}

BOOL TimelineDlg::IsValidInsFrame() {
	if (!ExistActiveDocument())
		return FALSE;
	return GetTargetStage()->IsFrameSel();
}	

void TimelineDlg::OnDelFrame() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	if (!IsValidDelFrame())
		return;
	
	Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	Layer* layer = stage->GetLayer(stage->GetSelLayer());
	int nStart;
	int nEnd;
	stage->GetSel(nStart,nEnd);
	nEnd = min(nEnd,layer->GetFrameCount()-1);
	ASSERT(nStart <= nEnd);
	GetDocument()->DoCmd(new DelFramesCmd(layer,nStart,nEnd - nStart + 1));
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateDelFrame(CCmdUI* pCmd) {
	pCmd->Enable(IsValidDelFrame());
}

BOOL TimelineDlg::IsValidDelFrame() {
	if (!ExistActiveDocument())
		return FALSE;

	if (!GetTargetStage()->IsFrameSel())
		return FALSE;

	Layer* layer = GetTargetStage()->GetLayer(GetTargetStage()->GetSelLayer());
	if (layer->GetFrameCount() <= GetTargetStage()->GetSelFrame())
		return FALSE;

	return TRUE;
}

void TimelineDlg::OnInsKeyFrame() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	if (!IsValidInsKeyFrame())
		return;

	Layer* layer = GetTargetStage()->GetLayer(GetTargetStage()->GetSelLayer());
	int selFrame = GetTargetStage()->GetSelFrame();
	GetDocument()->DoCmd(new InsKeyFrameCmd(layer,
											selFrame,
											layer->GenerateKeyFrame(selFrame,FALSE)));
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateInsKeyFrame(CCmdUI* pCmd) {
	pCmd->Enable(IsValidInsKeyFrame());
}

BOOL TimelineDlg::IsValidInsKeyFrame() {
	if (!ExistActiveDocument())
		return FALSE;

	if (!GetTargetStage()->IsFrameSel())
		return FALSE;
	return TRUE;
}
	
void TimelineDlg::OnInsEmptyKeyFrame() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	if (!IsValidInsEmptyKeyFrame())
		return;

	Layer* layer = GetTargetStage()->GetLayer(GetTargetStage()->GetSelLayer());
	int selFrame = GetTargetStage()->GetSelFrame();

	GetDocument()->DoCmd(new InsKeyFrameCmd(layer,
											selFrame,
											layer->GenerateKeyFrame(selFrame,TRUE)));

	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateInsEmptyKeyFrame(CCmdUI* pCmd) {
	pCmd->Enable(IsValidInsEmptyKeyFrame());
}

BOOL TimelineDlg::IsValidInsEmptyKeyFrame() {
	if (!ExistActiveDocument())
		return FALSE;

	return IsValidInsKeyFrame();
}

void TimelineDlg::OnInsLabel() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	KeyFrame* key;
	if (!IsValidInsLabel(key)) {
		return;
	}
	
	KeyFrameLabel label;
	key->GetLabel(&label);
	KeyFrameLabelDlg dlg(GetDocument()->appli.get(),&label,this);
	if (dlg.DoModal() == IDOK) {
		label = dlg.editLabel;
		label.useLabel = TRUE;
		GetDocument()->DoCmd(new InsKeyFrameLabelCmd(key,&label));
		GetDocument()->UpdateAll(NULL);
	}
}

void TimelineDlg::OnDelLabel() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	KeyFrame* key = NULL;
	if (!IsValidDelLabel(key)) {
		return;
	}
	GetDocument()->DoCmd(new DelKeyFrameLabelCmd(key));
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnModLabel() {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	KeyFrame* key = NULL;
	if (!IsValidModLabel(key)) {
		return;
	}

	KeyFrameLabel label;
	key->GetLabel(&label);
	KeyFrameLabelDlg dlg(GetDocument()->appli.get(),&label,this);
	if (dlg.DoModal() == IDOK) {
		GetDocument()->DoCmd(new ModKeyFrameLabelCmd(key,&dlg.editLabel));
		GetDocument()->UpdateAll(NULL);
	}
}

void TimelineDlg::OnUpdateInsLabel(CCmdUI* pcmd) {
	KeyFrame* infoFrame;
	pcmd->Enable(IsValidInsLabel(infoFrame));
}

void TimelineDlg::OnUpdateDelLabel(CCmdUI* pcmd) {
	KeyFrame* infoFrame;
	pcmd->Enable(IsValidDelLabel(infoFrame));
}

void TimelineDlg::OnUpdateModLabel(CCmdUI* pcmd) {
	KeyFrame* infoFrame;
	pcmd->Enable(IsValidModLabel(infoFrame));
}

BOOL TimelineDlg::GetSelKeyFrame(KeyFrame const*& key,BOOL bExact) const {
	if (!ExistActiveDocument())
		return FALSE;

	BOOL bEnabled = GetTargetStage()->IsFrameSel();
	if (bEnabled)
	{
		const Layer* layer = GetTargetStage()->GetLayer(GetTargetStage()->GetSelLayer());

		int selFrame = GetTargetStage()->GetSelFrame();
		key = layer->GetKeyFrame(selFrame);

		if (key == NULL)
			bEnabled = FALSE;
		else if (bExact && key->GetFramePos()  != selFrame)
			bEnabled = FALSE;
	}
	
	return bEnabled;
}

BOOL TimelineDlg::IsValidInsLabel(KeyFrame const*& key) const {
	if (!ExistActiveDocument())
		return FALSE;

	if (!GetSelKeyFrame(key,TRUE))
		return FALSE;

	return !key->HasLabel();
}

BOOL TimelineDlg::IsValidDelLabel(KeyFrame const*& key) const {
	if (!ExistActiveDocument())
		return FALSE;

	if (!GetSelKeyFrame(key,TRUE))
		return FALSE;

	return key->HasLabel();
}

BOOL TimelineDlg::IsValidModLabel(KeyFrame const*& key) const {
	if (!ExistActiveDocument())
		return FALSE;

	if (!GetSelKeyFrame(key,TRUE))
		return FALSE;

	return key->HasLabel();
}


/**
	カーソルキーによる選択フレームの移動
*/
void TimelineDlg::OnMoveDown() {
	if (!ExistActiveDocument())
		return;

	if (!IsValidMoveDown())
		return;

	GetTargetStage()->SelectLayer(GetTargetStage()->GetSelLayer()+1);
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnMoveUp() {
	if (!ExistActiveDocument())
		return;

	if (!IsValidMoveUp())
		return;

	GetTargetStage()->SelectLayer(GetTargetStage()->GetSelLayer()-1);
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnMoveRight() {
	if (!ExistActiveDocument())
		return;

	if (!IsValidMoveRight())
		return;

	int n = GetTargetStage()->GetSelFrame()+1;
	GetTargetStage()->SelectFrame(n);
	GetDocument()->player->SetPlayFrame(n);
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnMoveLeft() {
	if (!ExistActiveDocument())
		return;

	if (!IsValidMoveLeft())
		return;

	int n = GetTargetStage()->GetSelFrame()-1;
	GetTargetStage()->SelectFrame(n);
	GetDocument()->player->SetPlayFrame(n);
	GetDocument()->UpdateAll(NULL);
}

void TimelineDlg::OnUpdateMoveDown(CCmdUI* pcmd) {
	pcmd->Enable(IsValidMoveDown());
}

void TimelineDlg::OnUpdateMoveUp(CCmdUI* pcmd) {
	pcmd->Enable(IsValidMoveUp());
}

void TimelineDlg::OnUpdateMoveRight(CCmdUI* pcmd) {
	pcmd->Enable(IsValidMoveRight());
}

void TimelineDlg::OnUpdateMoveLeft(CCmdUI* pcmd) {
	pcmd->Enable(IsValidMoveLeft());
}

BOOL TimelineDlg::IsValidMoveDown() {
	if (!ExistActiveDocument())
		return FALSE;

	int nSel = GetTargetStage()->GetSelLayer();
	return nSel != -1 && nSel < (GetTargetStage()->GetLayerCount()-1);
}

BOOL TimelineDlg::IsValidMoveUp() {
	if (!ExistActiveDocument())
		return FALSE;
	int nSel = GetTargetStage()->GetSelLayer();
	return nSel != -1 && 0 < nSel;
}

BOOL TimelineDlg::IsValidMoveRight() {
	if (!ExistActiveDocument())
		return FALSE;

	int nSel = GetTargetStage()->GetSelFrame();
	return nSel = -1 && nSel < (GetTargetStage()->GetFrameCount()-1);
}

BOOL TimelineDlg::IsValidMoveLeft() {
	if (!ExistActiveDocument())
		return FALSE;

	int nSel = GetTargetStage()->GetSelFrame();
	return nSel = -1 && 0 < nSel;
}




	
