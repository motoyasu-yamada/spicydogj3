#include "stdafx.h"
#include "resource.h"
#include "MovCharCnt.h"

#include "cmd/stage/InsKeyFrameCmd.h"
#include "cmd/stage/ModKeyFrameCmd.h"
#include "model/stage/Stage.h"
#include "model/stage/Layer.h"
#include "model/stage/KeyFrame.h"
#include "SpicydogDoc.h"
#include "Player.h"
#include "utility/Tracker.h"
#include "utility/WndUtility.h"
#include "view/StageView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(MovCharCnt,CCmdTarget)
	ON_COMMAND(ID_MOVE_LEFT,	OnMoveLeft)
	ON_COMMAND(ID_MOVE_RIGHT,	OnMoveRight)
	ON_COMMAND(ID_MOVE_UP,		OnMoveUp)
	ON_COMMAND(ID_MOVE_DOWN,	OnMoveDown)
	ON_UPDATE_COMMAND_UI(ID_MOVE_LEFT,	OnUpdateMove)
	ON_UPDATE_COMMAND_UI(ID_MOVE_RIGHT, OnUpdateMove)
	ON_UPDATE_COMMAND_UI(ID_MOVE_UP,	OnUpdateMove)
	ON_UPDATE_COMMAND_UI(ID_MOVE_DOWN,	OnUpdateMove)
END_MESSAGE_MAP()

MovCharCnt::MovCharCnt() {
}

void MovCharCnt::OnEnter() {
	GetViewT()->Invalidate();
}

void MovCharCnt::OnRButtonDown(UINT,CPoint) {
	GetDocumentT()->BeginEdit();
	TrackNewPopupMenu(GetViewT()->GetTopLevelFrame(),IDM_STAGE);
}

void MovCharCnt::OnLButtonDown(UINT,CPoint pt) {
	OnLClick(pt);
	OnLDrag(pt);
}

void MovCharCnt::OnLClick(CPoint ptDP) {
	GetDocumentT()->BeginEdit();

	BOOL bUpdate = FALSE;
	Stage*  stage  = GetTargetStage();
	Player* player = GetDocumentT()->player.get();
	if (stage->GetSelFrame() != player->GetPlayFrame()) {
		stage->SelectFrame(player->GetPlayFrame());
		bUpdate = TRUE;
	}

	CPoint ptLP = ptDP;
	GetViewT()->DPtoLP(&ptLP);
	int nLayer = stage->HitFrameCharLayer(stage->GetSelFrame(),ptLP);
	if (stage->GetSelLayer() != nLayer) {
		if (nLayer == -1) {
			stage->UnselectLayer();
		} else {
			Layer* layer = stage->GetLayer(nLayer);
			if (!layer->IsSelected()) {
				stage->SelectLayer(nLayer);
			}
			int selFrame = stage->GetSelFrame();
			const KeyFrame* pBeginKeyFrame = layer->GetBeginKeyFrame(selFrame);
			if (pBeginKeyFrame != NULL && pBeginKeyFrame->GetFramePos() != selFrame) {
				KeyFrame* pNewKey = layer->GenerateKeyFrame(selFrame,FALSE);
				GetDocumentT()->DoCmd(new InsKeyFrameCmd(layer,selFrame,pNewKey));
			}
		}
		bUpdate = TRUE;
	}
	if (bUpdate)
		GetDocumentT()->UpdateAll(NULL);
}

void MovCharCnt::TrackMoveSelLayer(CPoint ptDP) {
	Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	Layer* layer = stage->GetLayer(stage->GetSelLayer());
	ASSERT_VALID(layer);
	CBEZIER bezier;
	layer->GetBezier(stage->GetSelFrame(),bezier);
	GetViewT()->LPtoDP(bezier);
	MoveBezierTracker tracker;
	tracker.SetTracker(&bezier);
	if (tracker.TrackMove(GetViewT(),ptDP)) {
		CSize sz = tracker.GetMoveOffset();
		GetViewT()->DPtoLP(&sz);
		MoveChar(sz.cx,sz.cy);
	}
}

void MovCharCnt::TrackSelNewLayer(CPoint ptDP) {
	Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	CRectTracker tracker;
	if (tracker.TrackRubberBand(GetViewT(),ptDP,TRUE)) {
		GetViewT()->DPtoLP(tracker.m_rect);
		int nSelLayer = stage->HitFrameCharLayer(stage->GetSelFrame() ,tracker.m_rect);
		if (nSelLayer != -1) {
			stage->SelectLayer(nSelLayer);
			GetDocumentT()->UpdateAll(NULL);
		}
	}
}

void MovCharCnt::OnLDrag(CPoint ptwnd) {
	if (GetTargetStage()->GetSelLayer() != -1) {
		TrackMoveSelLayer(ptwnd);
	} else {
		TrackSelNewLayer(ptwnd);
	}
}

BOOL MovCharCnt::OnSetCursor(CWnd*,UINT nHitTest,UINT) {
	if (nHitTest == HTCLIENT) {
		SetCursor(AfxGetApp()->LoadCursor(IDC_SEL_MOVE));
		return TRUE;
	}
	return GetViewT()->Default();
}

void MovCharCnt::OnMoveUp() {
	GetDocumentT()->BeginEdit();
	MoveChar(0,-1);
}

void MovCharCnt::OnMoveDown() {
	GetDocumentT()->BeginEdit();
	MoveChar(0,1);
}

void MovCharCnt::OnMoveRight() {
	GetDocumentT()->BeginEdit();
	MoveChar(1,0);
}

void MovCharCnt::OnMoveLeft() {
	GetDocumentT()->BeginEdit();
	MoveChar(-1,0);
}

void MovCharCnt::MoveChar(int x,int y) {
	if (!GetViewT()->IsValidMove())
		return;

	Stage* stage = GetTargetStage();
	int nSelLayer = stage->GetSelLayer();
	int selFrame = stage->GetSelFrame();
	Layer* layer = stage->GetLayer(nSelLayer);

	KeyFrame* key = layer->GetKeyFrame(selFrame);
	if (key == NULL) {
		return;
	}
	ASSERT_VALID(key);
	KeyFrameState state;
	key->GetState(&state);
	state.placing.x += x;
	state.placing.y += y;
	GetDocumentT()->DoCmd(new ModKeyFrameCmd(key,&state));
	GetDocumentT()->UpdateAll(NULL);
}

void MovCharCnt::OnUpdateMove(CCmdUI* pcmd) {
	pcmd->Enable(GetViewT()->IsValidMove());
}

void MovCharCnt::OnDraw(CDC* pDC) {
	if (GetDocumentT()->player->IsPlaying()) {
		return;
	}

	Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	int nSelLayer = stage->GetSelLayer();
	if (nSelLayer == -1) {
		return;
	}
	int selFrame = stage->GetSelFrame();
	if (selFrame == -1) {
		return;
	}
	Layer* layer = stage->GetLayer(nSelLayer);
	ASSERT_VALID(layer);
	CBEZIER bezier;
	layer->GetBezier(selFrame,bezier);
	
	CPen pen(PS_SOLID,0,RGB(0x00,0x00,0xFF));
	CPen* saved = pDC->SelectObject(&pen);
	ASSERT(saved != NULL);
	PolyBezier(pDC,bezier);
	VERIFY(pDC->SelectObject(saved));
}

void MovCharCnt::OnLButtonDblClk(UINT,CPoint) {
	Char* pChar;
	if (!GetViewT()->IsValidEditChar(pChar)) {
		return;
	}
	GetViewT()->OnEditChar();
}