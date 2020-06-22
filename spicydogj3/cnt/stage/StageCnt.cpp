#include "stdafx.h"
#include "StageCnt.H"
#include "SpicydogDoc.h"
#include "model/Appli.h"
#include "model/char/Char.h"
#include "model/stage/Stage.h"
#include "model/stage/Layer.h"
#include "model/stage/KeyFrame.h"
#include "model/stage/Placing.h"
#include "view/StageView.h"
#include "utility/Tracker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Stage* StageCnt::GetTargetStage() {
	return GetDocumentT()->appli->GetActiveStage();
}

KeyFrame* StageCnt::GetSelKeyFrame() {
	Stage* stage = GetTargetStage();
	if (stage == NULL) {
		ASSERT(FALSE);
		return NULL;
	}
	ASSERT_VALID(stage);
	int nSelLayer = stage->GetSelLayer();
	if (nSelLayer == -1) {
		ASSERT(FALSE);
		return NULL;
	}
	Layer* layer = stage->GetLayer(nSelLayer);
	int selFrame = stage->GetSelFrame();
	if (selFrame == -1) {
		ASSERT(FALSE);
		return NULL;
	}
	KeyFrame* key = layer->GetKeyFrame(selFrame);
	if (key == NULL) {
		ASSERT(FALSE);
		return NULL;
	}
	return key;
}

BOOL StageCnt::SetTracker(TrackerEx& tracker) {
	GetDocumentT()->BeginEdit();

	const Stage* stage = GetTargetStage();
	
	const int nSelLayer = stage->GetSelLayer();
	if (nSelLayer == -1) {
		ASSERT(FALSE);
		return FALSE;
	}
	const Layer* layer = stage->GetLayer(nSelLayer);
	ASSERT_VALID(layer);
	if (layer->GetLock()) {
		ASSERT(FALSE);
		return FALSE;
	}

	const int selFrame = stage->GetSelFrame();
	if (selFrame == -1) {
		ASSERT(FALSE);
		return FALSE;
	}

	const KeyFrame* key = layer->GetKeyFrame(selFrame);
	if (key == NULL) {
		return FALSE;
	}

	const Char* aChar = layer->GetAttachedChar();
	if (aChar == NULL) {
		return FALSE;
	}
	ASSERT_VALID(aChar);
	ASSERT(aChar->GetCharType().canAttachLayer);

	KeyFrameState state;
	key->GetState(&state);
	Placing& placingOld = state.placing;

	CBEZIER bezier;
	aChar->GetBezier(bezier);

	CRect rcLP;
	GetCBezierCircumscriptionRect(bezier,rcLP);
	rcLP.OffsetRect(placingOld.x,placingOld.y);
	CRect rcDP = rcLP;
	GetViewT()->LPtoDP(&rcDP);

	CPoint ptDP(placingOld.x,placingOld.y);
	GetViewT()->LPtoDP(&ptDP);

	tracker.SetTracker(rcDP,ptDP,placingOld.cx,placingOld.cy,placingOld.radian);
	return TRUE;
}
