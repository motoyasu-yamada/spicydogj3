#include "stdafx.h"
#include "resource.h"
#include "TimelineDlg.h"
#include "TimelineDlgConf.h"

#include "cmd/stage/RenLayerCmd.h"
#include "dlg/stage/RenLayerDlg.h"
#include "model/Appli.h"
#include "model/AppliConf.h"
#include "model/char/Char.h"
#include "model/stage/KeyFrame.h"
#include "model/stage/KeyFrameOwners.h"
#include "model/stage/Layer.h"
#include "model/stage/Stage.h"
#include "Player.h"
#include "SpicydogDoc.h"
#include "utility/StringUtility.h"
#include "utility/PaintMemDC.h"
#include "utility/ScrollControler.h"
#include "utility/WndUtility.h"
#include "wnd/AppliFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(TimelineDlg, DialogBarDlg)
	ON_COMMAND(ID_ACTIONS_PROP, OnActionsProp)
	ON_COMMAND(ID_DEL_LAYER,					OnDelLayer)	
	ON_COMMAND(ID_DUP_LAYER,					OnDupLayer)	
	ON_COMMAND(ID_EDIT_CLEAR,					OnDelLayer)	
	ON_COMMAND(ID_FRAME_DEL,		OnDelFrame)
	ON_COMMAND(ID_FRAME_INS,		OnInsFrame)
	ON_COMMAND(ID_HIDE_OTHER_LAYERS,				OnHideOtherLayers)	
	ON_COMMAND(ID_INS_LAYER,					OnInsLayer)	
	ON_COMMAND(ID_KEYFRAME_INS,		OnInsKeyFrame)	
	ON_COMMAND(ID_KEYFRAME_INSEMPTY,OnInsEmptyKeyFrame)
	ON_COMMAND(ID_LABEL_DEL, OnDelLabel)
	ON_COMMAND(ID_LABEL_INS, OnInsLabel)
	ON_COMMAND(ID_LABEL_MOD, OnModLabel)
	ON_COMMAND(ID_LOCK_OTHER_LAYERS,				OnLockOtherLayers)	
	ON_COMMAND(ID_MOVE_DOWN,	OnMoveDown)
	ON_COMMAND(ID_MOVE_LEFT,	OnMoveLeft)
	ON_COMMAND(ID_MOVE_RIGHT,	OnMoveRight)
	ON_COMMAND(ID_MOVE_UP,		OnMoveUp)
	ON_COMMAND(ID_ORDER_DOWN,					OnOrderDown)
	ON_COMMAND(ID_ORDER_UP,						OnOrderUp)	
	ON_COMMAND(ID_REN_LAYER,					OnRenLayer)	
	ON_COMMAND(ID_SHOW_ALL_LAYERS,				OnShowAllLayers)	
	ON_COMMAND(ID_TWEEN_CREATE,		OnTweenCreate)
	ON_COMMAND(ID_TWEEN_PROPERTY,	OnTweenProperty)
	ON_UPDATE_COMMAND_UI(ID_ACTIONS_PROP, OnUpdateActionsProp)
	ON_UPDATE_COMMAND_UI(ID_DEL_LAYER,			OnUpdateDelLayer)
	ON_UPDATE_COMMAND_UI(ID_DUP_LAYER,			OnUpdateDupLayer)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR,			OnUpdateDelLayer)
	ON_UPDATE_COMMAND_UI(ID_FRAME_DEL,			OnUpdateDelFrame)
	ON_UPDATE_COMMAND_UI(ID_FRAME_INS,			OnUpdateInsFrame)
	ON_UPDATE_COMMAND_UI(ID_HIDE_OTHER_LAYERS,   OnUpdateHideOtherLayers)
	ON_UPDATE_COMMAND_UI(ID_INS_LAYER,			OnUpdateInsLayer)
	ON_UPDATE_COMMAND_UI(ID_KEYFRAME_INS,		OnUpdateInsKeyFrame)		
	ON_UPDATE_COMMAND_UI(ID_KEYFRAME_INSEMPTY,	OnUpdateInsEmptyKeyFrame)
	ON_UPDATE_COMMAND_UI(ID_LABEL_DEL, OnUpdateDelLabel)
	ON_UPDATE_COMMAND_UI(ID_LABEL_INS, OnUpdateInsLabel)
	ON_UPDATE_COMMAND_UI(ID_LABEL_MOD, OnUpdateModLabel)
	ON_UPDATE_COMMAND_UI(ID_LOCK_OTHER_LAYERS,   OnUpdateLockOtherLayers)
	ON_UPDATE_COMMAND_UI(ID_MOVE_DOWN,	OnUpdateMoveDown)
	ON_UPDATE_COMMAND_UI(ID_MOVE_LEFT,	OnUpdateMoveLeft)
	ON_UPDATE_COMMAND_UI(ID_MOVE_RIGHT, OnUpdateMoveRight)
	ON_UPDATE_COMMAND_UI(ID_MOVE_UP,	OnUpdateMoveUp)
	ON_UPDATE_COMMAND_UI(ID_ORDER_DOWN,			OnUpdateOrderDown)
	ON_UPDATE_COMMAND_UI(ID_ORDER_UP,			OnUpdateOrderUp)
	ON_UPDATE_COMMAND_UI(ID_REN_LAYER,			OnUpdateRenLayer)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ALL_LAYERS,		OnUpdateShowAllLayers)
	ON_UPDATE_COMMAND_UI(ID_TWEEN_CREATE,		OnUpdateTweenCreate)
	ON_UPDATE_COMMAND_UI(ID_TWEEN_PROPERTY,		OnUpdateTweenProperty)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_SAFX_SPLITH_MOVE()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

const COLORREF COLOR_IMAGELISTMASK = RGB(0x00,0x80,0x80);
const LPCTSTR FONTFACE_LAYERHDR  = _T("ÇlÇr ÇoÉSÉVÉbÉN");
const LPCTSTR FONTFACE_FRAMEHDR  = _T("ÇlÇr ÇoÉSÉVÉbÉN");
const int FONTPOINT_LAYERHDR   = 100;
const int FONTPOINT_FRAMEHDR   = 80;

CImageList f_imagelistLayer;
CImageList f_imagelistChar;
CPen    f_penGrid(PS_SOLID,0,COLOR_OF_GRID);
CPen    f_penPlaying(PS_SOLID,0,COLOR_OF_PLAY);
CFont	f_fontFrameHdr;
CFont	f_fontLayerHdr;

IMPLEMENT_DYNCREATE(TimelineDlg,DialogBarDlg)
TimelineDlg::TimelineDlg()
:	DialogBarDlg(),
	m_pDocument(NULL),
	m_bInited(FALSE) {
	
	clinit();
}

void TimelineDlg::clinit() {
	static BOOL clinited = FALSE;
	if (clinited) {
		return;
	}
	CreateImageListColor32(f_imagelistLayer,IDB_LAYERICON,SIZE_OF_LAYER_BMP.cx,SIZE_OF_LAYER_BMP.cy,COLOR_IMAGELISTMASK);
	CreateImageListColor32(f_imagelistChar ,IDB_LAYERCHAR,SIZE_OF_CHAR_BMP.cx, SIZE_OF_CHAR_BMP.cy ,COLOR_IMAGELISTMASK);
	f_fontLayerHdr.CreatePointFont(FONTPOINT_LAYERHDR,FONTFACE_LAYERHDR);
	f_fontFrameHdr.CreatePointFont(FONTPOINT_FRAMEHDR,FONTFACE_FRAMEHDR);
	clinited = TRUE;
}

TimelineDlg::~TimelineDlg() {
}

void TimelineDlg::DoDataExchange(CDataExchange* pDX) {
	DialogBarDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDCS_V, m_sbV);
	DDX_Control(pDX, IDCS_H, m_sbH);
	DDX_Control(pDX, IDCS_SPLITTER, m_Splitter);
}

BOOL TimelineDlg::OnInitDialog() {
	DialogBarDlg::OnInitDialog();

	m_Splitter.SetLeftPaneMinWidth(64);
	m_Splitter.SetRightPaneMinWidth(128);

	m_nStartFrame = 0;
	m_nStartLayer = 0;

	AdjustSize();

	VERIFY(m_Target.Register(this));
	m_bInited = TRUE;

	return TRUE;
}

void TimelineDlg::OnDocumentActivated(CDocument* document) {
	DialogBarDlg::OnDocumentActivated(document);
	if (!m_bInited)
		return;

	m_pDocument = DYNAMIC_DOWNCAST(SpicydogDoc,document);
	BOOL b = m_pDocument != NULL;
	m_wndFrame.EnableWindow(b);
	m_wndLayer.EnableWindow(b);
	m_itbFrame.EnableWindow(b);
	m_itbLayer.EnableWindow(b);
	m_sbH.EnableWindow(b);
	m_sbV.EnableWindow(b);
	if (!b) {
		m_wndFrame.Invalidate(FALSE);
		m_wndLayer.Invalidate(FALSE);
	}
}

void TimelineDlg::OnDocumentUpdated(CDocument* document,LPARAM lHint, CObject* pHint)  {
	DialogBarDlg::OnDocumentUpdated(document,lHint,pHint);
	if (!m_bInited)
		return;
	
	if (!(lHint & SpicydogDoc::UPDATED_PLAY)) {
		UpdateFrameScroll();
		UpdateLayerScroll();
	}
	m_wndFrame.Invalidate(FALSE);
	m_wndLayer.Invalidate(FALSE);
}

void TimelineDlg::UpdateFrameScroll() {
	if (!ExistActiveDocument()) {
		m_sbH.EnableWindow(FALSE);
		return;
	} else {
		m_sbH.EnableWindow(TRUE);
	}

	m_nFramePage = (m_rcFrames.Width() + SIZE_OF_CELL.cx - 1)/ SIZE_OF_CELL.cx;
	m_nFrameMax = GetDrawFrameCount() -1;

	Stage* stage = GetTargetStage();
	int nSel = stage != NULL ? stage->GetSelFrame() : -1;
	if (nSel != -1)
	{
		if (nSel < m_nStartFrame)
			m_nStartFrame = nSel;
		if ((m_nStartFrame + m_nFramePage -1 ) <= nSel)
			m_nStartFrame = nSel - m_nFramePage + 2;
	}

	SCROLLINFO info;
	ZeroMemory(&info,sizeof info);
	info.cbSize = sizeof SCROLLINFO;
	info.fMask  = SIF_PAGE | SIF_POS | SIF_RANGE;
	if (GetDrawFrameCount() < m_nFramePage)
		info.fMask |= SIF_DISABLENOSCROLL;
	info.nMin   = 0;
	info.nMax   = m_nFrameMax;
	info.nPage  = m_nFramePage;
	info.nPos   = m_nStartFrame;

	m_sbH.SetScrollInfo(&info,TRUE);
}

void TimelineDlg::UpdateLayerScroll() {
	if (!ExistActiveDocument()) {
		m_sbV.EnableWindow(FALSE);
		return;
	} else {
		m_sbV.EnableWindow(TRUE);
	}

	int nLayers = GetTargetStage()->GetLayerCount();
	m_nLayerPage = m_rcLayers.Height() / SIZE_OF_CELL.cy;
	m_nLayerMax  = max(GetTargetStage()->GetLayerCount()-1,m_nLayerPage);

	Stage* stage = GetTargetStage();
	int nSel = stage != NULL ? stage->GetSelLayer() : -1;
	if (0 <= nSel)
	{
		if ((m_nStartLayer + m_nLayerPage - 1) <= nSel)
			m_nStartLayer = nSel - m_nLayerPage + 2;
		if (nSel < m_nStartLayer)
			m_nStartLayer = nSel;
	}

	SCROLLINFO info;
	ZeroMemory(&info,sizeof info);
	info.cbSize = sizeof SCROLLINFO;
	info.fMask  = SIF_PAGE | SIF_POS | SIF_RANGE;
	if (nLayers == 0 || nLayers < m_nLayerPage)
		info.fMask |= SIF_DISABLENOSCROLL;
	info.nMin   = 0;
	info.nMax   = m_nLayerMax;
	if (info.nMax <= 0)
		info.nMax = 0;
	info.nPage  = m_nLayerPage;
	info.nPos   = m_nStartLayer;

	m_sbV.SetScrollInfo(&info,TRUE);
}

void TimelineDlg::OnSize(UINT nType, int cx, int cy)  {
	DialogBarDlg::OnSize(nType, cx, cy);
	if (!m_bInited)
		return;

	//VERIFY(LockWindowUpdate());
		AdjustSize();
		UpdateFrameScroll();
		UpdateLayerScroll();
	//UnlockWindowUpdate();

	Invalidate(FALSE);
}

void TimelineDlg::OnSplitHMove() {
	//VERIFY(LockWindowUpdate());
		AdjustSize();
		UpdateFrameScroll();
		UpdateLayerScroll();
	//UnlockWindowUpdate();
	Invalidate(FALSE);
}

const int CONTROLS_MARGIN = 1;
void TimelineDlg::AdjustSize() {
	CRect rc;
	GetClientRect(rc);
	const int nSbHH = GetWindowHeight(m_sbH);
	const int nSbVW = GetWindowWidth(m_sbV);
	const int nITBH = GetWindowHeight(m_itbFrame);

	const int rightLeftPane = GetChildWindowLeft(m_Splitter);

	StretchChildWindowToBottom(m_Splitter,rc.bottom);
	StretchChildWindowToBottom(m_sbV,rc.bottom-nITBH);
	MoveChildWindowToTop(m_sbH,rc.bottom - nITBH);

	StretchChildWindowToRight(m_wndLayer, rightLeftPane);
	StretchChildWindowToBottom(m_wndLayer,GetChildWindowTop(m_sbH) - CONTROLS_MARGIN);

	MoveChildWindowToLeftNext(m_wndFrame,m_Splitter);
	StretchChildWindowToRight(m_wndFrame,rc.right-nSbVW);
	StretchChildWindowToBottom(m_wndFrame,GetChildWindowTop(m_sbH) - CONTROLS_MARGIN);

	MoveChildWindowToBottom(m_itbLayer,rc.bottom);
	StretchChildWindowToRight(m_itbLayer,rightLeftPane);
	MoveChildWindowToLeftNext(m_itbFrame,m_Splitter);
	MoveChildWindowToBottom(m_itbFrame, rc.bottom);

	MoveChildWindowToRight(m_sbV,rc.right);
	MoveChildWindowToLeftNext(m_sbH,m_itbFrame);
	StretchChildWindowToRight(m_sbH,rc.right-nSbVW);
	
	m_wndLayer.GetClientRect(m_rcLayers);
	m_wndFrame.GetClientRect(m_rcFrames);

	m_rcLayerHdr = CRect(m_rcLayers.left,m_rcLayers.top,m_rcLayers.right,HEIGHT_OF_HDR);
	m_rcLayerList= CRect(m_rcLayers.left,m_rcLayers.top + HEIGHT_OF_HDR,m_rcLayers.right,m_rcLayers.bottom);

	m_rcFrameHdr = CRect(m_rcFrames.left,m_rcFrames.top,m_rcFrames.right,HEIGHT_OF_HDR);
	m_rcFrameList= CRect(m_rcFrames.left,m_rcFrames.top + HEIGHT_OF_HDR,m_rcFrames.right,m_rcFrames.bottom);
}

SpicydogDoc* TimelineDlg::GetDocument() {
	ASSERT_VALID(m_pDocument);
	return m_pDocument;
}

BOOL TimelineDlg::PreCreateWindow(CREATESTRUCT& cs)  {
	if (!DialogBarDlg::PreCreateWindow(cs))
		return FALSE;

	cs.style |= CS_DBLCLKS;

	return TRUE;
}

int TimelineDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)  {
	if (DialogBarDlg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_itbFrame.Create(IDT_FRAME,this,-1,WS_CHILD|WS_VISIBLE,1);
	m_itbLayer.Create(IDT_LAYER,this,-1,WS_CHILD|WS_VISIBLE,2);

	m_wndLayer.Create(this,this,NULL,3,WS_CHILD | WS_VISIBLE | WS_BORDER,WS_EX_CLIENTEDGE);
	m_wndFrame.Create(this,this,NULL,4,WS_CHILD | WS_VISIBLE | WS_BORDER,WS_EX_CLIENTEDGE);

	return 0;
}

int TimelineDlg::GetLayerIndexFromY(int yClient) const {
	int yLayer = yClient - m_rcLayerList.top;
	if (yLayer < 0)
		return -1;

	int nLayer =  m_nStartLayer + yLayer / SIZE_OF_CELL.cy;
	if (0 <= nLayer && nLayer < GetTargetStage()->GetLayerCount())
		return nLayer;

	return -1;
}

//	ÉNÉäÉbÉNÇµÇΩà íuÇ…ëŒâûÇ∑ÇÈÉtÉåÅ[ÉÄÇéÊìæÇ∑ÇÈ
int TimelineDlg::GetFrameIndexFromX(int yClient) const {
	int yFrame = yClient - m_rcFrameList.left;

	int nFrame =  m_nStartFrame + yFrame / SIZE_OF_CELL.cx;
	if (0 <= nFrame && nFrame < GetDrawFrameCount())
		return nFrame;

	return -1;
}

int TimelineDlg::GetDrawFrameCount() const {
	if (ExistActiveDocument())
		return GetTargetStage()->GetFrameCount() + CELL_OF_EXCESS;
	else
		return m_nStartFrame + m_nFramePage;
}

void TimelineDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)  {
	ScrollControler cont(this,m_nStartLayer,nSBCode,nPos,pScrollBar,m_nLayerMax,m_nLayerPage);
	cont.Scroll();
	m_wndLayer.Invalidate();
	m_wndFrame.Invalidate();
}

void TimelineDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)  {
	ScrollControler cont(this,m_nStartFrame,nSBCode,nPos,pScrollBar,m_nFrameMax,m_nFramePage);
	cont.Scroll();
	m_wndFrame.Invalidate();
}


void TimelineDlg::OnPaint(InWnd* pwnd) {
	PaintMemDC dc(pwnd);
	CRect rc;
	pwnd->GetClientRect(rc);
	dc.FillSolidRect(rc,::GetSysColor(COLOR_APPWORKSPACE));
	if (pwnd == &m_wndLayer) {
		DrawLayerHdr(dc);
	} else {
		ASSERT(pwnd == &m_wndFrame);
		DrawFrameHdr(dc);
	}

	if (ExistActiveDocument()) {
		if (pwnd == &m_wndLayer) {
			DrawLayerList(dc);
		} else {
			ASSERT(pwnd == &m_wndFrame);
			DrawFrameList(dc);
		}
	}
}

BOOL TimelineDlg::IsLayerWnd(CWnd* pComp) const
{
	HWND hwndComp = pComp != NULL ? pComp->GetSafeHwnd() : NULL;
	ASSERT(hwndComp == m_wndLayer.GetSafeHwnd() || hwndComp == m_wndFrame.GetSafeHwnd());

	return hwndComp == m_wndLayer.GetSafeHwnd();
}

BOOL TimelineDlg::IsFrameWnd(CWnd* pComp) const
{
	HWND hwndComp = pComp != NULL ? pComp->GetSafeHwnd() : NULL;
	ASSERT(hwndComp == m_wndLayer.GetSafeHwnd() || hwndComp == m_wndFrame.GetSafeHwnd());

	return hwndComp == m_wndFrame.GetSafeHwnd();
}

void TimelineDlg::OnLButtonDblClk(InWnd* pChild,UINT,CPoint point) {
	if (!ExistActiveDocument())
		return;

	GetDocument()->BeginEdit();

	if (!IsLayerWnd(pChild)) {
		return;
	}

	int nDownLayer = GetLayerIndexFromY(point.y);
	if (nDownLayer == -1) {
		return;
	}
	GetTargetStage()->SelectLayer(nDownLayer);

	Layer* layer = GetTargetStage()->GetLayer(nDownLayer);
	RenLayerDlg dlg(layer->GetName(),this);
	if (dlg.DoModal() == IDOK) {
		GetDocument()->DoCmd(new RenLayerCmd(layer,dlg.GetName()));
		GetDocument()->UpdateAll(NULL);
	}
}

//	ÉRÉìÉeÉLÉXÉgÉÅÉjÉÖÅ[ÇÃï\é¶
void TimelineDlg::OnRButtonDown(InWnd* pChild,UINT nFlags, CPoint point)  {
	if (!ExistActiveDocument()) {
		return;
	}

	GetDocument()->BeginEdit();

	BOOL bInLayerRect = IsLayerWnd(pChild);
	BOOL bInFrameRect = !IsLayerWnd(pChild);
	ASSERT(!bInFrameRect || !bInLayerRect);

	if (bInLayerRect)
	{
		SelectLayer(point);
		GetDocument()->UpdateAll(NULL);
	}
	else
		SelectFrame(point,FALSE);

	if (bInFrameRect || bInLayerRect)
	{
		CMenu menu;
		if (bInFrameRect)
			VERIFY(menu.LoadMenu(IDM_TIMELINE));
		else
			VERIFY(menu.LoadMenu(IDM_LAYER));

		CMenu* pMenu = menu.GetSubMenu(0);
		if (pMenu != NULL)
		{
			CPoint ptScreen;
			VERIFY(GetCursorPos(&ptScreen));
			VERIFY(pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN,ptScreen.x,ptScreen.y,GetTopLevelFrame()));
		}
	}
	else
		DialogBarDlg::OnRButtonDown(nFlags, point);
}

inline BOOL IsClickedOnSelFrames(int nStart,int nEnd,int nClick)
{
	if (nEnd == -1)
		return nStart == nClick;
	else if (nStart == -1)
		return FALSE;
	else
	{
		ASSERT(nStart <= nEnd);
		if (nStart <= nClick && nClick <= nEnd)
			return TRUE;
		else
			return FALSE;
	}
}

BOOL TimelineDlg::SelectFrame(CPoint point,BOOL bLDown) {
	if (!ExistActiveDocument())
		return FALSE;

	BOOL bSelected = FALSE;

	Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);

	int nSelFrame;
	int nSelEndFrame;
	stage->GetSel(nSelFrame,nSelEndFrame);

	int nDownLayer = GetLayerIndexFromY(point.y);
	if (nDownLayer == -1) {
		if (stage->GetSelLayer() != -1) {
			stage->UnselectLayer();
			bSelected = TRUE;
		}
	} else {
		Layer* layer = stage->GetLayer(nDownLayer);

		if (stage->GetSelLayer() != nDownLayer) {
			stage->SelectLayer(nDownLayer);
			if (nSelFrame != -1)
				stage->SelectFrame(nSelFrame);
			bSelected = TRUE;
		}
	}

	int nDownFrame = GetFrameIndexFromX(point.x);
	if (GetKeyStillDown(VK_SHIFT)) {
		stage->AddSel(nDownFrame);
		bSelected = TRUE;
	} else {
		if (bLDown || !IsClickedOnSelFrames(nSelFrame,nSelEndFrame,nDownFrame)) {
			stage->SelectFrame(nDownFrame);
			GetDocument()->player->SetPlayFrame(nDownFrame);
			m_wndFrame.Invalidate();
			bSelected = TRUE;
		}
	}

	return bSelected;
}

TimelineDlg::HIT_LAYER TimelineDlg::GetLayerItemFromX(int x) const {
	int l = m_rcLayerList.left;
	if (x < (l + OFFSET_OF_LAYER_NAME.cx))
		return HL_ICON;

	int r = m_rcLayerList.right;
	if ((r + ROFFSET_OF_LAYER_LCK.cx) < x)
		return HL_LOCK;
	if ((r + ROFFSET_OF_LAYER_EYE.cx) < x)
		return HL_EYE;
	
	return HL_NAME;
}

BOOL TimelineDlg::SelectLayer(CPoint point) {
	if (!ExistActiveDocument())
		return FALSE;

	BOOL bSelected = FALSE;
	Stage* stage = GetTargetStage();

	int nDownLayer = GetLayerIndexFromY(point.y);
	if (nDownLayer == -1) {
		if (stage->GetSelLayer() != -1) {
			stage->UnselectLayer();
			bSelected = TRUE;
		}
	} else {
		HIT_LAYER hit = GetLayerItemFromX(point.x);

		Layer* layer = stage->GetLayer(nDownLayer);

		switch(hit) {
		case HL_LOCK: 
			layer->SetLock(!layer->GetLock());
			if (layer->GetLock() && layer->IsSelected()) {
				stage->UnselectLayer();
				bSelected = TRUE;
			}
			break;
		case HL_EYE:
			layer->SetVisible(!layer->GetVisible());
			break;
		case HL_ICON: {
			Char* pChar = layer->GetAttachedChar();
			if (pChar == NULL) {
				break;
			}
			CView* pView = GetActiveView();
			ASSERT_VALID(pView);
			AppliFrameWnd* pMovieFrame = (AppliFrameWnd*)DYNAMIC_DOWNCAST(AppliFrameWnd,pView->GetParentFrame());
			ASSERT_VALID(pMovieFrame);
			pMovieFrame->OpenCharGUI(layer->GetAttachedChar());
			}
			break;
		}

		int nCurSel = stage->GetSelLayer();
		//if (hit != HL_NAME || 
		if (nCurSel != nDownLayer && !layer->GetLock()) {
			stage->SelectLayer(nDownLayer);
			bSelected = TRUE;
		}
	}

	return bSelected;
}

void TimelineDlg::OnLButtonDown(InWnd* pChild,UINT nFlags, CPoint point)  {
	if (!ExistActiveDocument()) {
		return;
	}

	GetDocument()->BeginEdit();

	UNUSED_ALWAYS(nFlags);

	if (IsLayerWnd(pChild))
	{
		if (!IsToStartDrag(pChild,point,TRUE)) {
			SelectLayer(point);
			GetDocument()->UpdateAll(NULL);
		} else {
			SelectLayer(point);
			Stage* stage = GetTargetStage();
			ASSERT_VALID(stage);
			int nSel = stage->GetSelLayer();
			if (0 <= nSel) {
				ASSERT_VALID(&m_Source);
				if (GetTargetStage()->SubmitOle(&m_Source,TRUE,GetDocument(),nSel))
				{
					DROPEFFECT de = m_Source.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);
					ASSERT(de == DROPEFFECT_NONE || de == DROPEFFECT_COPY || de == DROPEFFECT_MOVE);
					PostMessage(WM_LBUTTONUP,nFlags,MAKELONG(point.x,point.y));
				}
			}
		}
	} else {
		SelectFrame(point,TRUE);
		GetDocument()->UpdateAll(NULL);
	}
}

Stage* TimelineDlg::GetTargetStage() {
	return GetDocument()->appli->GetActiveStage();
}

const Stage* TimelineDlg::GetTargetStage() const {
	return const_cast<TimelineDlg*>(this)->GetDocument()->appli->GetActiveStage();
}

void TimelineDlg::CalcDragRect(int nSel) {
	if (nSel == -1) {
		m_rcDrag.SetRect(m_rcLayerList.left,m_rcLayerList.top-1,m_rcLayerList.right,m_rcLayerList.top+1);
	} else {
		int nY = m_rcLayerList.top + nSel * SIZE_OF_CELL.cy;
		m_rcDrag.SetRect(m_rcLayerList.left,nY,m_rcLayerList.right,nY+SIZE_OF_CELL.cy);
	}
}

void TimelineDlg::DrawLayerHdr(CDC& dc) {
	CRect rc = m_rcLayerHdr;
	rc.bottom++;
	dc.FillSolidRect(rc,COLOR_OF_HDR);
	dc.Draw3dRect(rc,COLOR_OF_HILIGHT,COLOR_OF_SHADOW);
	
	dc.SetTextColor(COLOR_OF_HDRTXT);
	dc.SetBkMode(TRANSPARENT);
	
	CFont* saved = dc.SelectObject(&f_fontLayerHdr);
	ASSERT_VALID(saved);
	
	CString strFrame;
	CString strSecond;
	if (ExistActiveDocument()) {
		const Player* player = GetDocument()->player.get();
		const AppliConf& ac  = *GetDocument()->appli->appliConf.get();
		const int nFPS = ac.frameRate;
		const int nPlayFrame = player->GetPlayFrame();
		const int nTotalFrame= player->GetLastFrame();
		const int nCurrentSecs = (nPlayFrame * 1000) / nFPS;
		const int nTotalSecs   = (nTotalFrame * 1000) / nFPS;
		strFrame  = FormatString(IDS_LAYERHDR_FRAME_FORMAT,nPlayFrame + 1,nTotalFrame + 1);
		strSecond = FormatString(IDS_LAYERHDR_TIME_FORMAT, nCurrentSecs,nTotalSecs);
	} else {
		strSecond = strFrame = _T("-/-");
	}

	int nLeft = 1;
	int nTop  = 1;
	f_imagelistLayer.Draw(&dc,LAYERICON_FRAME,CPoint(nLeft,nTop),ILD_TRANSPARENT);
	nLeft += SIZE_OF_LAYER_BMP.cx + 2;

	CRect rcFrame(nLeft,nTop,nLeft+dc.GetTextExtent(strFrame).cx,nTop+SIZE_OF_LAYER_BMP.cy);
	dc.DrawText(strFrame,rcFrame,DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	nLeft += rcFrame.Width() + 2;

	f_imagelistLayer.Draw(&dc,LAYERICON_TIME,CPoint(nLeft,nTop),ILD_TRANSPARENT);
	nLeft += SIZE_OF_LAYER_BMP.cx + 2;
	CRect rcSecond(nLeft,nTop,nLeft+dc.GetTextExtent(strSecond).cx,nTop+SIZE_OF_LAYER_BMP.cy);
	dc.DrawText(strSecond,rcSecond,DT_SINGLELINE | DT_VCENTER | DT_LEFT);
//exit:
	VERIFY(dc.SelectObject(saved) != NULL);
}

void Draw3dLine(CDC& dc,int x1,int y1,int x2,int y2,int ox,int oy,COLORREF cr1,COLORREF cr2) {
	CPen pen1;
	CPen pen2;
	VERIFY(pen1.CreatePen(PS_SOLID,1,cr1));
	VERIFY(pen2.CreatePen(PS_SOLID,1,cr2));
	CPen* saved = dc.SelectObject(&pen1);
	ASSERT_VALID(saved);

	dc.MoveTo(x1,y1);
	dc.LineTo(x2,y2);

	dc.SelectObject(&pen2);
	dc.MoveTo(x1 + ox,y1 + oy);
	dc.LineTo(x2 + ox,y2 + oy);
	
	VERIFY(dc.SelectObject(saved) != NULL);
}

void Draw3dLine(CDC& dc,CPoint pt1,CPoint pt2,CPoint ptOff,COLORREF cr1,COLORREF cr2) {
	Draw3dLine(dc,pt1.x,pt1.y,pt2.x,pt2.y,ptOff.x,ptOff.y,cr1,cr2);
}

void DrawLayerIcon(CDC& dc,CPoint ptLayer,int nType) {
	ASSERT_VALID(&dc);
	f_imagelistChar.Draw(&dc,nType,ptLayer + OFFSET_OF_LAYER_CHAR,ILD_TRANSPARENT);
}

void DrawLayerName(CDC& dc,LPCTSTR ptszName,LPCRECT prc)
{
	ASSERT_VALID(&dc);
	ASSERT(AfxIsValidAddress(prc,sizeof RECT,FALSE));
	dc.DrawText(ptszName,lstrlen(ptszName),const_cast<LPRECT>(prc),DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
}

void DrawLayerEye(CDC& dc,CPoint ptRT,BOOL bEye)
{
	ASSERT_VALID(&dc);

	CPoint pt = ptRT + ROFFSET_OF_LAYER_EYE;
	CPoint pt1 = pt;
	pt1.x -= 2;
	CPoint pt2 = pt1 + CSize(0,SIZE_OF_CELL.cy - ROFFSET_OF_LAYER_EYE.cy * 2);
	Draw3dLine(dc,pt1,pt2,CPoint(1,0),GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DHILIGHT));

	if (bEye)
		f_imagelistLayer.Draw(&dc,LAYERICON_EYE,pt,ILD_TRANSPARENT);
}

void DrawLayerLck(CDC& dc,CPoint ptRT,BOOL bLock,BOOL bSel)
{
	ASSERT_VALID(&dc);
	CPoint pt = ptRT + ROFFSET_OF_LAYER_LCK;
	CPoint pt1 = pt;
	pt1.x -= 2;
	CPoint pt2 = pt1 + CSize(0,SIZE_OF_CELL.cy - ROFFSET_OF_LAYER_LCK.cy * 2);
	Draw3dLine(dc,pt1,pt2,CPoint(1,0),GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DHILIGHT));

	if (bLock)
		f_imagelistLayer.Draw(&dc,LAYERICON_LOCK,pt,ILD_TRANSPARENT);
	else if (bSel)
		f_imagelistLayer.Draw(&dc,LAYERICON_EDIT,pt,ILD_TRANSPARENT);
}


void TimelineDlg::DrawLayerList(CDC& dc) {
	ASSERT_VALID(&dc);

	dc.FillSolidRect(m_rcLayerList,::GetSysColor(COLOR_APPWORKSPACE));

	CFont* saved = dc.SelectObject(&f_fontLayerHdr);
	ASSERT_VALID(saved);
	dc.SetBkMode(TRANSPARENT);

	int nMaxLayer = GetTargetStage()->GetLayerCount();
	int nY = m_rcLayerList.top; 
	for (int n = m_nStartLayer;n < nMaxLayer;n++) {
		BOOL bSel = GetTargetStage()->GetSelLayer() == n;
		CRect rcLayer = CRect(0,nY + 1,m_rcLayers.right,nY + SIZE_OF_CELL.cy+1);
		CPoint ptRT = CPoint(rcLayer.right,rcLayer.top);
		CPoint ptLT = rcLayer.TopLeft();
		Layer* layer = GetTargetStage()->GetLayer(n);
		ASSERT_VALID(layer);
		dc.FillSolidRect(rcLayer,COLOR_OF_LAYER);
		dc.Draw3dRect(rcLayer,COLOR_OF_HILIGHT,COLOR_OF_SHADOW);

		CRect rcName;
		rcName.TopLeft() = rcLayer.TopLeft() + OFFSET_OF_LAYER_NAME;
		rcName.right = rcLayer.right + ROFFSET_OF_LAYER_NAME.cx;
		rcName.bottom = rcLayer.bottom - OFFSET_OF_LAYER_NAME.cy;
		rcName.DeflateRect(0,1);
		if (bSel)
			dc.FillSolidRect(rcName,COLOR_OF_LAYER_SEL);

		if (bSel)
			dc.SetTextColor(COLOR_OF_LAYERTEXT_SEL);
		else
			dc.SetTextColor(COLOR_OF_LAYERTEXT);
		DrawLayerName(dc,layer->GetName(),rcName);
		Char* pChar = layer->GetAttachedChar();
		DrawLayerIcon(dc,ptLT,(pChar == 0) ? 0 : pChar->GetCharType().iconIndex);
		DrawLayerEye(dc,ptRT,layer->GetVisible());
		DrawLayerLck(dc,ptRT,layer->GetLock(),bSel);

		nY += SIZE_OF_CELL.cy;
	}
	
	VERIFY(dc.SelectObject(saved) != NULL);
}

void TimelineDlg::DrawFrameHdr(CDC& dc) {

	dc.FillSolidRect(m_rcFrameHdr,COLOR_OF_HDR);
	dc.Draw3dRect(m_rcFrameHdr,COLOR_OF_HILIGHT,COLOR_OF_SHADOW);
	
	CFont* savedFont = dc.SelectObject(&f_fontFrameHdr);
	CPen*  savedPen  = dc.SelectObject(&f_penGrid);
	ASSERT_VALID(savedFont);
	ASSERT_VALID(savedPen);

	dc.SetTextColor(COLOR_OF_HDRTXT);
	dc.SetBkMode(TRANSPARENT);

	int nDrawMaxFrame = GetDrawFrameCount();
	int nX = m_rcFrameHdr.left;
	for (int nFrame = m_nStartFrame;nFrame < nDrawMaxFrame;nFrame++) {
		dc.MoveTo(nX,m_rcFrameHdr.top + 1);
		dc.LineTo(nX,m_rcFrameHdr.top + 3);
		dc.MoveTo(nX,m_rcFrameHdr.bottom - 2);
		dc.LineTo(nX,m_rcFrameHdr.bottom - 4);
		
		BOOL bSFrame = nFrame == 0 || (nFrame + 1) % 5 == 0; 
		if (bSFrame) {
			CString str;
			str.Format(_T("%d"),nFrame + 1);
			VERIFY(dc.TextOut(nX + 2,4,str));
		}
		nX += SIZE_OF_CELL.cx;		
		if (m_rcFrameHdr.right < nX)
			break;
	}

	VERIFY(dc.SelectObject(savedPen) != NULL);
	VERIFY(dc.SelectObject(savedFont) != NULL);
}


void FrameSolidRect(CDC& dc,int x, int y, int cx, int cy,COLORREF c) {
	dc.FillSolidRect(x, y, cx - 1, 1, c);
	dc.FillSolidRect(x, y, 1, cy - 1, c);
	dc.FillSolidRect(x + cx, y, -1, cy, c);
	dc.FillSolidRect(x, y + cy, cx, -1, c);
}

void FrameSolidRect(CDC& dc,LPCRECT lpRect,COLORREF cr) {
	FrameSolidRect(dc,lpRect->left, lpRect->top, 
			          lpRect->right  - lpRect->left,
		              lpRect->bottom - lpRect->top,
			          cr);
}

//	É^ÉCÉÄÉâÉCÉìï`âÊ
//	óLå¯Ç»îwåiìhÇËÇ¬Ç‘Çµ
//	ñ≥å¯Ç»îwåiìhÇËÇ¬Ç‘Çµ
//	ÉOÉäÉbÉhï`âÊ
//	ÉLÅ[ÉtÉåÅ[ÉÄ(òg,ÉAÉCÉRÉì)ï`âÊ
void DrawKey(CDC& dc,CPoint ptCell) {
	CRect rc(ptCell + OFFSET_OF_KEY,SIZE_OF_KEY);
	dc.FillSolidRect(rc,COLOR_OF_KEY);
}

void DrawKeyE(CDC& dc,CPoint ptCell) {
	CRect rc(ptCell + OFFSET_OF_KEY,SIZE_OF_KEY);
	dc.FillSolidRect(rc,COLOR_OF_KEYE_FILL);
	FrameSolidRect(dc,rc,COLOR_OF_KEYE_FRAM);
}

void DrawAct(CDC& dc,CPoint ptCell) {
	CRect rc(ptCell + OFFSET_OF_ACT,SIZE_OF_ACT);
	dc.FillSolidRect(rc,COLOR_OF_ACT);
}

void DrawLab(CDC& dc,CPoint ptCell) {
	CRect rc(ptCell + OFFSET_OF_LAB,SIZE_OF_LAB);
	dc.FillSolidRect(rc,COLOR_OF_LAB);
}

void FillSolidPoint(CDC& dc,CPoint pt,COLORREF cr) {
	CRect rc(pt,CSize(1,1));
	dc.FillSolidRect(rc,cr);
}

void DrawEnd(CDC& dc,CPoint ptCell) {
	CPoint pt = ptCell + OFFSET_OF_END;
	
	FillSolidPoint(dc,pt,COLOR_OF_FRAME_END);
	pt += CSize(1,1);
	FillSolidPoint(dc,pt,COLOR_OF_FRAME_END);
	pt += CSize(1,1);
	FillSolidPoint(dc,pt,COLOR_OF_FRAME_END);
	
	pt += CSize(0,-2);
	FillSolidPoint(dc,pt,COLOR_OF_FRAME_END);
	pt += CSize(-1,1);
	FillSolidPoint(dc,pt,COLOR_OF_FRAME_END);
	pt += CSize(-1,1);
	FillSolidPoint(dc,pt,COLOR_OF_FRAME_END);
}

void TimelineDlg::GetCell(CPoint& pt,int nLayer,int nFrame) {
	pt.x = m_rcFrameList.left + (nFrame - m_nStartFrame) * SIZE_OF_CELL.cx;
	pt.y = m_rcFrameList.top  + (nLayer - m_nStartLayer) * SIZE_OF_CELL.cy;
}

void TimelineDlg::DrawFrameList(CDC& dc) {
	int nDrawMaxFrame = GetDrawFrameCount();
	int nMaxLayer = GetTargetStage()->GetLayerCount();
	int nPlayFrame = GetDocument()->player->GetPlayFrame();
	int nSelFrame  = GetTargetStage()->GetSelFrame();
	int nLayersToDraw = nMaxLayer - m_nStartLayer;
	int yEndLayerBottom = m_rcFrameList.top + nLayersToDraw * SIZE_OF_CELL.cy;

	{
		CRect rcCells = m_rcFrameList;
		rcCells.bottom = yEndLayerBottom;
		dc.FillSolidRect(rcCells,COLOR_OF_CELL_FILL);
	}

	{
		CRect rcCellXs = m_rcFrameList;
		rcCellXs.top = yEndLayerBottom;
		dc.FillSolidRect(rcCellXs,COLOR_OF_CELLX_FILL);
	}

	CPen* saved = dc.SelectObject(&f_penGrid);
	ASSERT_VALID(saved);
	for (int nY = m_rcFrameList.top;nY <= m_rcFrameList.bottom;nY += SIZE_OF_CELL.cy) {
		dc.MoveTo(m_rcFrameList.left,nY);
		dc.LineTo(m_rcFrameList.right,nY);
	}
	
	for (int nX = m_rcFrameList.left;nX <= m_rcFrameList.right;nX += SIZE_OF_CELL.cx) {
		dc.MoveTo(nX,m_rcFrameList.top);
		dc.LineTo(nX,m_rcFrameList.bottom);
	}
	VERIFY(dc.SelectObject(saved) != NULL);

	nY = m_rcFrameList.top;
	for (int nLayer = m_nStartLayer;nLayer < nMaxLayer;nLayer++)
	{
		Layer* layer = GetTargetStage()->GetLayer(nLayer);
		BOOL bSelLayer = (nLayer == GetTargetStage()->GetSelLayer());
		
		KeyFrameOwners& os = layer->GetKeyFrameOwners();
		KeyFrameOwners::OWNERS::const_iterator i = os.Begin();
	
		while (i != os.End())
		{
			const KeyFrame* pKey = *i;
			ASSERT_VALID(pKey);
			i++;
			const KeyFrame* pKeyNext = NULL;
			if (i != os.End())
			{
				pKeyNext = *i;
				ASSERT_VALID(pKeyNext);
			}
			int nPos = pKey->GetFramePos();
			int nNextPos = (pKeyNext != NULL) ? pKeyNext->GetFramePos() : layer->GetFrameCount();

			CPoint ptCell;
			GetCell(ptCell,nLayer,nPos);
			if (!pKey->IsEmpty()) {
				CRect rcRange(ptCell,CSize(SIZE_OF_CELL.cx * (nNextPos - nPos) + 1,SIZE_OF_CELL.cy + 1));
				KeyFrameState state;
				pKey->GetState(&state);
				if (!state.animating.animating) {
					dc.FillSolidRect(rcRange,COLOR_OF_CELLV_FILL);
				} else {
					dc.FillSolidRect(rcRange,COLOR_OF_CELLT_FILL);
				}
				FrameSolidRect(dc,rcRange,COLOR_OF_CELLV_FRAME);
			}
		}

		if (layer->IsSelected() && nSelFrame != -1)
		{
			int nStart;
			int nEnd;
			GetTargetStage()->GetSel(nStart,nEnd);
			for (int n = nStart;n <= nEnd;n++)
			{
				CPoint ptCell;
				GetCell(ptCell,nLayer,n);
				CRect rc(ptCell,SIZE_OF_CELL);
				rc.DeflateRect(1,1,0,0);
				dc.FillSolidRect(rc,COLOR_OF_CELL_SEL);
			}
		}

		for (i = os.Begin();i != os.End();i++)
		{
			const KeyFrame* pKey = *i;
			ASSERT_VALID(pKey);

			int nPos = pKey->GetFramePos();
			CPoint ptCell;
			GetCell(ptCell,nLayer,nPos);

			if (pKey->IsEmpty())
				DrawKeyE(dc,ptCell);
			else
				DrawKey(dc,ptCell);	

			if (pKey->HasAction())
				DrawAct(dc,ptCell);
			if (pKey->HasLabel())
				DrawLab(dc,ptCell);
		}

		CPoint ptEndCell;
		GetCell(ptEndCell,nLayer,layer->GetFrameCount());
		DrawEnd(dc,ptEndCell);
	}

	CPen* savedPen = dc.SelectObject(&f_penPlaying);
	ASSERT_VALID(saved);
	int xPlay = (nPlayFrame - m_nStartFrame) * SIZE_OF_CELL.cx + SIZE_OF_CELL.cx / 2;
	if (0 < xPlay && m_rcFrameList.right) {
		dc.MoveTo(xPlay,m_rcFrameList.top);
		dc.LineTo(xPlay,m_rcFrameList.bottom);
	}
	VERIFY(dc.SelectObject(savedPen) != NULL);
}

void TimelineDlg::OnDragLeave(CWnd* pWnd) {
	if (!ExistActiveDocument())
		return;

	CClientDC dc(&m_wndLayer);
	dc.DrawDragRect(NULL,    SIZE_OF_DRAG_RECT_BORDER,
		            m_rcDrag,SIZE_OF_DRAG_RECT_BORDER);
}

DROPEFFECT TestDropEffect(COleDataObject* pObj,DWORD dw) {
	DROPEFFECT de  = DROPEFFECT_NONE;
	if (pObj->IsDataAvailable(CF_CHARS) ||
        pObj->IsDataAvailable(CF_LAYERS))
	{
		de = dw & MK_CONTROL ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
	}
	return de;
}

DROPEFFECT TimelineDlg::OnDragOver(COleDataObject* pObj,DWORD dw,CPoint pt) {
	if (!ExistActiveDocument())
		return DROPEFFECT_NONE;

	CRect rcOld(m_rcDrag);
	CalcDragRect(GetLayerIndexFromY(pt.y));
	if (rcOld != m_rcDrag) {
		CClientDC dc(&m_wndLayer);
		dc.DrawDragRect(m_rcDrag,SIZE_OF_DRAG_RECT_BORDER,
						rcOld,   SIZE_OF_DRAG_RECT_BORDER);
	}
	return TestDropEffect(pObj,dw);
}

DROPEFFECT TimelineDlg::OnDragEnter(COleDataObject* pObj,DWORD dw,CPoint pt) {
	if (!ExistActiveDocument())
		return DROPEFFECT_NONE;

	CalcDragRect(GetLayerIndexFromY(pt.y));
	CClientDC dc(&m_wndLayer);
	dc.DrawDragRect(m_rcDrag,SIZE_OF_DRAG_RECT_BORDER,
		            NULL,    SIZE_OF_DRAG_RECT_BORDER);

	return TestDropEffect(pObj,dw);
}

BOOL TimelineDlg::OnDrop(COleDataObject* pObj,DROPEFFECT de,CPoint pt) {
	if (!ExistActiveDocument())
		return FALSE;

	if (pObj == NULL)
		{ ASSERT(FALSE);return FALSE; }
	
	Stage::OLE_ACCEPT_PARAM param;
	param.m_bDnD = TRUE;
	param.m_Method = Stage::OLE_ACCEPT_PARAM::methodOver;
	param.m_nLayerPos = GetLayerIndexFromY(pt.y);
	param.m_ptDropPos = CPoint(0,0);

	CPoint ptF = pt;
	ClientToScreen(&ptF);
	m_wndFrame.ScreenToClient(&ptF);
	if (m_rcFrames.PtInRect(ptF))
		param.m_nFramePos = GetFrameIndexFromX(ptF.x);
	else
		param.m_nFramePos = -1;
	
	param.m_nFrameLen = 1;

	Stage* stage = GetTargetStage();
	ASSERT_VALID(stage);
	stage->AcceptOle(pObj,de,GetDocument(),param);

	return TRUE;
}

