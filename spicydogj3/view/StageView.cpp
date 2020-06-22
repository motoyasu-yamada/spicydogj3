#include "stdafx.h"
#include "resource.h"
#include "StageView.h"

#include "cnt/ScrollViewCnt.h"
#include "cnt/stage/MovCharCnt.h"
#include "dlg/stage/EditPlacingDlg.h"
#include "dlg/stage/EditColoringDlg.h"
#include "model/Appli.h"
#include "model/AppliConf.h"
#include "model/stage/Stage.h"
#include "model/stage/KeyFrame.h"
#include "model/AffineMatrix.h"
#include "Player.h"
#include "SpicydogDoc.h"
#include "SpicydogPref.h"
#include "utility/StringUtility.h"
#include "wnd/AppliFrameWnd.h"
#include "cmd/stage/DelLayerCmd.h"
#include "cmd/stage/ModKeyFrameCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(StageView, ScrollViewEx)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_PLAY_FIRST,  OnPlayFirst)
	ON_COMMAND(ID_PLAY_LAST,   OnPlayLast)
	ON_COMMAND(ID_PLAY_NEXT,   OnPlayNext)
	ON_COMMAND(ID_PLAY_PLAY,   OnPlayPlay)
	ON_COMMAND(ID_PLAY_PREV,   OnPlayPrev)
	ON_COMMAND(ID_PLAY_STOP,   OnPlayStop)
	ON_COMMAND(ID_SHOW_CENTER,  OnShowCenter)
	ON_COMMAND(ID_EDIT_COLORING,OnEditColoring)
	ON_COMMAND(ID_EDIT_PLACING, OnEditPlacing)
	ON_COMMAND(ID_EDIT_CHAR,    OnEditChar)
	ON_COMMAND(ID_EDIT_CLEAR,   OnDelLayer)
	ON_COMMAND_EX(ID_STAGEVIEW_NOMASK,OnMaskView)
	ON_COMMAND_EX(ID_STAGEVIEW_MASK,  OnMaskView)
	ON_UPDATE_COMMAND_UI(ID_PLAY_LAST,   OnUpdatePlayLast)
	ON_UPDATE_COMMAND_UI(ID_PLAY_FIRST,  OnUpdatePlayFirst)
	ON_UPDATE_COMMAND_UI(ID_PLAY_NEXT,   OnUpdatePlayNext)
	ON_UPDATE_COMMAND_UI(ID_PLAY_PLAY,   OnUpdatePlayPlay)
	ON_UPDATE_COMMAND_UI(ID_PLAY_PREV,   OnUpdatePlayPrev)
	ON_UPDATE_COMMAND_UI(ID_PLAY_STOP,   OnUpdatePlayStop)
	ON_UPDATE_COMMAND_UI(ID_SHOW_CENTER,  OnUpdateShowCenter)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PLACING, OnUpdateEditPlacing)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COLORING,OnUpdateEditColoring)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CHAR,    OnUpdateEditChar)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR,   OnUpdateDelLayer)
	ON_UPDATE_COMMAND_UI(ID_STAGEVIEW_NOMASK,OnUpdateMaskView)
	ON_UPDATE_COMMAND_UI(ID_STAGEVIEW_MASK,  OnUpdateMaskView)
END_MESSAGE_MAP()

const UINT IDTIMER_PLAY = 0x0001;
BOOL StageView::s_bHideCenter = FALSE;

IMPLEMENT_DYNCREATE(StageView, ScrollViewEx)
StageView::StageView()
:	m_bMaskView(FALSE),m_bNeedToScrollToCenter(FALSE) {
	ScrollViewEx::Center(TRUE);
}

StageView::~StageView() {
}

BOOL StageView::GetViewCaption(CString& caption) const {
	const Stage* stage = GetTargetStage();
	caption = stage->GetStageName();
	return TRUE;
}

void StageView::OnInitialUpdate() {
	ScrollViewEx::OnInitialUpdate();

	ScrollViewEx::SetAccelerators(IDA_STAGE);

	VERIFY(m_Target.Register(this));
	AppendControler(ID_TOOL_SELECT, new MovCharCnt);
	AppendControler(ID_SCRVIEW_ZOOM,new ScrollViewZoomCnt);
	AppendControler(ID_SCRVIEW_MOVE,new ScrollViewMoveCnt);
	SetDefaultControlerID(ID_TOOL_SELECT);
	ActivateControler(ID_TOOL_SELECT);

	const AppliConf* appconf = GetDocument()->appli->appliConf.get();
	m_szStage = appconf->stageSize;
	ScrollViewEx::SetScrollSizes(m_szStage,4,CPoint(m_szStage.cx/2,m_szStage.cy/2));
	ScrollViewEx::SetScale(1);
	m_bNeedToScrollToCenter = TRUE;
}

void StageView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) {
	if (!IsInitialUpdated()) 
		return;

	ScrollViewEx::OnUpdate(pSender,lHint,pHint);

	const AppliConf* appconf = GetDocument()->appli->appliConf.get();
	const CSize szNew = appconf->stageSize;
	if (m_szStage != szNew) {
		m_szStage = szNew;
		ScrollViewEx::SetScrollSizes(m_szStage,4,CPoint(m_szStage.cx/2,m_szStage.cy/2));
	}

	AppliFrameWnd* pwnd = (AppliFrameWnd*)DYNAMIC_DOWNCAST(AppliFrameWnd,GetParentFrame());
	pwnd->UpdateViewSwitchWnd();
}

void StageView::OnDraw(CDC* pDC) {
	CBrush brBkgnd   (RGB(0xC0,0xC0,0xC0));
	CBrush brBorder  (RGB(0x00,0x00,0x00));
	CBrush brDocument(GetDocument()->appli->appliConf->bkgndColor);

	ScrollViewEx::FillBkgnd(pDC,&brBkgnd);
	ScrollViewEx::FillDocumentRect(pDC,&brDocument);

	pDC->SetBkMode(TRANSPARENT);
	if (ACTIVE_PREF.DisplayPixelGrid && 4 < GetScale()) {
		CPen pen(PS_DOT,0,ACTIVE_PREF.PixelGridColor);
		ScrollViewEx::DrawGridHelper(pDC,pen,1);	
	}
	if (ACTIVE_PREF.DisplayGrid && 0.4 < GetScale()) {
		CPen pen(PS_DOT,0,ACTIVE_PREF.GridColor);
		ScrollViewEx::DrawGridHelper(pDC,pen,ACTIVE_PREF.GridInterval);	
	}
	if (ACTIVE_PREF.DisplayCenterGrid) {
		CPen pen(PS_SOLID,0,ACTIVE_PREF.CenterGridColor);
		ScrollViewEx::DrawCenterLine(pDC,pen);	
	}
	
	ScrollViewEx::DrawDocumentBorder(pDC,&brBorder);

	if (m_bMaskView) {
		ScrollViewEx::MaskDocumentRect(pDC,TRUE);
	}

	DrawChar(pDC);

	if (m_bMaskView) {
		ScrollViewEx::MaskDocumentRect(pDC,FALSE);
		ScrollViewEx::DrawDocumentBorder(pDC,&brBorder);
	}
}

BOOL StageView::OnMaskView(UINT nID) {
	if (nID == ID_STAGEVIEW_NOMASK)
		m_bMaskView = FALSE;
	else if (nID == ID_STAGEVIEW_MASK)
		m_bMaskView = TRUE;
	Invalidate();
	return TRUE;
}

void StageView::OnUpdateMaskView(CCmdUI* pCmdUI) {
	UINT nID = pCmdUI->m_nID;
	if (nID == ID_STAGEVIEW_NOMASK)
		pCmdUI->SetRadio(!m_bMaskView);
	else if (nID == ID_STAGEVIEW_MASK)
		pCmdUI->SetRadio(m_bMaskView);
}

Stage* StageView::GetTargetStage() {
	return GetDocument()->appli->GetActiveStage();
}

const Stage* StageView::GetTargetStage() const {
	return GetDocument()->appli->GetActiveStage();
}

void StageView::DrawChar(CDC* pDC) {
	const AppliConf* appconf = GetDocument()->appli->appliConf.get();
	CSize szFrame = appconf->stageSize;

	const Stage* stage = GetTargetStage();
	int nFrame = GetDocument()->player->GetPlayFrame();

	stage->Draw(nFrame,pDC);

	if (!s_bHideCenter) {
		for (int nLayer = stage->GetLayerCount()-1;0 <= nLayer;nLayer --) {
			const Layer* layer = stage->GetLayer(nLayer);
			ASSERT_VALID(layer);
			AffineMatrix matrix;
			Coloring coloring;
			if (layer->GetFrameInfo(nFrame,&matrix,&coloring,FALSE)) {
				CPoint ptDP(matrix.x,matrix.y);
				LPtoDP(&ptDP);
				DrawCenter(pDC,ptDP);
			}
		}
	}

	TWndControlerMgr<ScrollViewEx>::OnDraw(pDC);
}

SpicydogDoc* StageView::GetDocument() {
	SpicydogDoc* document = dynamic_cast<SpicydogDoc*>(CView::GetDocument());
	ASSERT_VALID(document);
	return document;
}

const SpicydogDoc* StageView::GetDocument() const {
	SpicydogDoc* document = dynamic_cast<SpicydogDoc*>(CView::GetDocument());
	ASSERT_VALID(document);
	return document;
}

void StageView::OnPlayFirst() {
	GetDocument()->player->FirstFrame();
	GetDocument()->UpdateAll(NULL,SpicydogDoc::UPDATED_PLAY);
}

void StageView::OnPlayLast() {
	GetDocument()->player->LastFrame();
	GetDocument()->UpdateAll(NULL,SpicydogDoc::UPDATED_PLAY);
}

void StageView::OnPlayNext() {
	GetDocument()->player->NextFrame();
	GetDocument()->UpdateAll(NULL,SpicydogDoc::UPDATED_PLAY);
}

void StageView::OnPlayPrev() {
	GetDocument()->player->PrevFrame();
	GetDocument()->UpdateAll(NULL,SpicydogDoc::UPDATED_PLAY);
}

void StageView::OnPlayPlay()  {
	ActivateDefaultControler();
	Player* player = GetDocument()->player.get();
	if (player->IsPlaying()) {
		return;
	}
	int nFrame = 1000 / GetDocument()->appli->appliConf->frameRate;
	player->Play();
	if (!SetTimer(IDTIMER_PLAY,nFrame,NULL)) {
		ASSERT(FALSE);
		player->Stop();
	}
}

void StageView::OnPlayStop() {
	Player* player = GetDocument()->player.get();
	if (player->IsPlaying()) {
		player->Stop();
		VERIFY(KillTimer(IDTIMER_PLAY));

		GetDocument()->UpdateAll(NULL);
	}
}

CString StageView::GetCaption() const {
	return LoadString(IDS_STAGEVIEW_CAPTION);
}

int StageView::GetIconIndex() const {
	return 0;
}

BOOL StageView::IsClosable() const {
	return FALSE;
}

void StageView::OnUpdatePlayLast(CCmdUI* pCmdUI) {
	UNUSED_ALWAYS(pCmdUI);
}

void StageView::OnUpdatePlayFirst(CCmdUI* pCmdUI) {
	UNUSED_ALWAYS(pCmdUI);
}

void StageView::OnUpdatePlayNext(CCmdUI* pCmdUI) {
	UNUSED_ALWAYS(pCmdUI);
}

void StageView::OnUpdatePlayPrev(CCmdUI* pCmdUI) {
	UNUSED_ALWAYS(pCmdUI);
}

void StageView::OnUpdatePlayStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->player->IsPlaying());
}
void StageView::OnUpdatePlayPlay(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->player->IsPlaying());
}

void StageView::OnTimer(UINT nIDEvent) {
	if (nIDEvent == IDTIMER_PLAY) {
		Player* player = GetDocument()->player.get();
		if (player->IsPlaying()) {
			player->NextFrame();
			GetDocument()->UpdateAll(NULL,SpicydogDoc::UPDATED_PLAY);
		} else {
			VERIFY(KillTimer(IDTIMER_PLAY));
			GetDocument()->UpdateAll(NULL);
		}
	}
}

void StageView::OnClose() {
	Player* player = GetDocument()->player.get();
	if (player->IsPlaying()) {
		player->Stop();
		VERIFY(KillTimer(IDTIMER_PLAY));
	}
}

void StageView::OnEditChar() {
	Char* aChar;
	if(!IsValidEditChar(aChar))
		return;
	ASSERT_VALID(aChar);
	AppliFrameWnd* appliFrame = (AppliFrameWnd*)DYNAMIC_DOWNCAST(AppliFrameWnd,GetParentFrame());
	ASSERT_VALID(appliFrame);
	appliFrame->OpenCharGUI(aChar);
}

void StageView::OnUpdateEditChar(CCmdUI* p) {
	Char* aChar;
	p->Enable(IsValidEditChar(aChar));
}

BOOL StageView::IsValidEditChar(Char*& aChar) {
	Stage* stage = GetTargetStage();
	int select = stage->GetSelLayer();
	if (select == -1) {
		return FALSE;
	}

	Layer* layer = stage->GetLayer(select);
	ASSERT_VALID(layer);
	aChar = layer->GetAttachedChar();
	return aChar != NULL;
}

void StageView::OnEditPlacing() {
	KeyFrame* key = NULL;
	if (!IsValidPlaceProp(key) || key == NULL) {
		return;
	}
	const Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	const Layer* layer = stage->GetLayer(stage->GetSelLayer());
	ASSERT_VALID(layer);
	const Char* aChar = layer->GetAttachedChar();
	BOOL bAllowSizing = aChar != NULL && aChar->GetCharType().canModifyPlacing;

	KeyFrameState state;
	key->GetState(&state);

	EditPlacingDlg dlg(bAllowSizing,state.placing,this);
	if (dlg.DoModal() == IDOK) {
		key->SetState(&state);
		GetDocument()->DoCmd(new ModKeyFrameCmd(key,&state));
		GetDocument()->UpdateAll(NULL);
	}
}

void StageView::OnUpdatePlaceProp(CCmdUI* pCmd) {
	KeyFrame* key = NULL;
	pCmd->Enable(IsValidPlaceProp(key));
}

BOOL StageView::IsValidPlaceProp(KeyFrame*& key) {	
	Stage* stage = GetTargetStage();
	if (!stage->IsFrameSel()) {
		return FALSE;
	}
	Layer* layer = stage->GetLayer(stage->GetSelLayer());
	ASSERT_VALID(layer);
	if (layer->GetFrameType(stage->GetSelFrame()) != Layer::FRAME_KEY) {
		return FALSE;
	}
	key = layer->GetKeyFrame(stage->GetSelFrame());

	return TRUE;
}


void StageView::OnUpdateEditColoring(CCmdUI* pCmd) {
	KeyFrame* key = NULL;
	pCmd->Enable(IsValidEditColoring(key));
}

void StageView::OnEditColoring() {
	KeyFrame* key = NULL;
	if (!IsValidEditColoring(key) || key == NULL) {
		return;
	}

	KeyFrameState state;
	key->GetState(&state);
	Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	Layer* layer = stage->GetLayer(stage->GetSelLayer());
	ASSERT_VALID(layer);

	EditColoringDlg dlg(layer->GetAttachedChar(),&state.coloring,this);
	if (dlg.DoModal() == IDOK) {
		key->SetState(&state);
		GetDocument()->DoCmd(new ModKeyFrameCmd(key,&state));
		GetDocument()->UpdateAll(NULL);
	}
}

void StageView::OnUpdateEditPlacing(CCmdUI* pCmd) {
	KeyFrame* key = NULL;
	pCmd->Enable(IsValidEditPlacing(key));
}

BOOL StageView::IsValidEditColoring(KeyFrame*& key) {
	if (!IsValidPlaceProp(key))
		return FALSE;

	const Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	const Layer* layer = stage->GetLayer(stage->GetSelLayer());
	ASSERT_VALID(layer);

	const Char* aChar = layer->GetAttachedChar();
	return aChar != NULL && aChar->GetCharType().canModifyColoring;
}

BOOL StageView::IsValidEditPlacing(KeyFrame*& key) {
	if (!IsValidPlaceProp(key))
		return FALSE;

	const Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	const Layer* layer = stage->GetLayer(stage->GetSelLayer());
	ASSERT_VALID(layer);

	const Char* aChar = layer->GetAttachedChar();
	return aChar != NULL && aChar->GetCharType().canModifyPlacing;
}

void StageView::OnShowCenter() {
	s_bHideCenter = !s_bHideCenter;
	Invalidate();
}

void StageView::OnUpdateShowCenter(CCmdUI* pcmd) {
	pcmd->SetCheck(s_bHideCenter ? 0 : 1);
}

BOOL StageView::IsValidMove() const {
	const Stage* stage = GetTargetStage();
	if (!stage->IsFrameSel())
		return FALSE;
	
	int nSelLayer = stage->GetSelLayer();
	const Layer* layer = stage->GetLayer(nSelLayer);
	if (layer->GetFrameCount() <= stage->GetSelFrame())
		return FALSE;

	return TRUE;

}

BOOL StageView::IsValidSizing() const {
	if (!IsValidMove())
		return FALSE;

	const Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	const Layer* layer = stage->GetLayer(stage->GetSelLayer());
	ASSERT_VALID(layer);
	const Char* aChar = layer->GetAttachedChar();
	return aChar != NULL && aChar->GetCharType().canModifyPlacing;
}

void StageView::OnDelLayer()  {
	GetDocument()->BeginEdit();

	if (!IsValidDelLayer()) {
		return;
	}


	GetDocument()->DoCmd(new DelLayerCmd(GetTargetStage(),GetTargetStage()->GetSelLayer()));
	GetDocument()->UpdateAll(NULL);
	Invalidate();
}

void StageView::OnUpdateDelLayer(CCmdUI* pCmd) {
	pCmd->Enable(IsValidDelLayer());
}

BOOL StageView::IsValidDelLayer() const {
	return GetTargetStage()->GetSelLayer()!= -1 && 1 < GetTargetStage()->GetLayerCount();
}

void StageView::OnSize(UINT nType, int cx, int cy) {
	ScrollViewEx::OnSize(nType, cx, cy);
	if (m_bNeedToScrollToCenter) {
		m_bNeedToScrollToCenter = FALSE;
		ScrollViewEx::ScrollToCenter();
	}	
}

DROPEFFECT StageView::OnDragEnter(COleDataObject* pObj,DWORD,CPoint) {
	if (pObj->IsDataAvailable(CF_CHARS))
		return DROPEFFECT_COPY;

	return DROPEFFECT_NONE;
}

DROPEFFECT StageView::OnDragOver(COleDataObject* pObj,DWORD dw,CPoint pt) {
	return OnDragEnter(pObj,dw,pt);
}

BOOL StageView::OnDrop(COleDataObject* pObj,DROPEFFECT de,CPoint pt) {
	if (pObj == NULL)
		{ ASSERT(FALSE); return FALSE;}

	SpicydogDoc* pDocument = GetDocument();
	if (pDocument == NULL)
		{ ASSERT(FALSE); return FALSE;}

	Stage* stage = GetTargetStage();
	Stage::OLE_ACCEPT_PARAM param;
	param.m_bDnD = TRUE;
	param.m_Method = Stage::OLE_ACCEPT_PARAM::methodIns;
	param.m_nLayerPos = 0;
	param.m_nFramePos = pDocument->player->GetPlayFrame();
	param.m_nFrameLen = 1;
	param.m_ptDropPos = pt;
	DPtoLP(&param.m_ptDropPos);

	if (!stage->AcceptOle(pObj,de,GetDocument(),param))
		return FALSE;

	CFrameWnd* pFrame = (CFrameWnd*)DYNAMIC_DOWNCAST(CFrameWnd,AfxGetMainWnd());
	if (pFrame == NULL)
		{ASSERT(FALSE);return FALSE;}
	ASSERT_VALID(pFrame);

	CControlBar* pControlBar = pFrame->GetControlBar(ID_VIEW_TIMELINE);
	if (pControlBar == NULL)
		{ASSERT(FALSE);return FALSE;}
	ASSERT_VALID(pControlBar);

	pControlBar->SetFocus();
	return TRUE;
}
