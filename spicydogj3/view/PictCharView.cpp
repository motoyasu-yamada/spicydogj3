#include "stdafx.h"
#include "resource.H"
#include "PictCharView.h"
#include "cmd/char/PictCharCmd.h"
#include "cnt/PictCnt.h"
#include "cnt/ScrollViewCnt.h"
#include "dlg/char/EditPictQuantizeDlg.h"
#include "dlg/char/PictEffectPictDlg.h"
#include "dlg/char/PictResizeDlg.h"
#include "dlg/char/PictTrimDlg.h"
#include "model/AffineMatrix.h"
#include "model/Appli.h"
#include "model/AppliConf.h"
#include "model/stage/Coloring.h"
#include "SpicydogDoc.h"
#include "SpicydogPref.h"
#include "utility/StringUtility.h"
#include "utility/LeadToolUtility.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(PictCharView,ScrollViewEx)
	ON_COMMAND(ID_COLOR_CHANGE_LINE, OnLineColorChanged)
	ON_COMMAND(ID_DEBUG,OutputPreview2File)
	ON_COMMAND(ID_INVERT_H,OnInvertH)
	ON_COMMAND(ID_INVERT_V,OnInvertV)
	ON_COMMAND(ID_PE_AVERAGE,OnPEAverage)
	ON_COMMAND(ID_PE_MEDIAN,OnPEMedian)
	ON_COMMAND(ID_PE_MOSAIC,OnPEMosaic)
	ON_COMMAND(ID_PE_OILPAINT,OnPEOilify)
	ON_COMMAND(ID_PE_POSTERIZE,OnPEPosterize)
	ON_COMMAND(ID_PE_SHARPNESS,OnPESharpness)
	ON_COMMAND(ID_PE_SOLARIZE,OnPESolarize)
	ON_COMMAND(ID_PICT_EDIT,OnEditByExe)
	ON_COMMAND(ID_PICT_EDITVIEW,OnEditview)
	ON_COMMAND(ID_PICT_MOUSE_TRIM,OnPictMouseTrim)
	ON_COMMAND(ID_PICT_OPT,OnPictOpt)
	ON_COMMAND(ID_PICT_PREVIEW,OnPreview)
	ON_COMMAND(ID_PICT_RELOAD,OnReload)
	ON_COMMAND(ID_PICT_RESET_SELRECT,OnResetSelRect)
	ON_COMMAND(ID_PICT_RESIZE,OnPictResize)
	ON_COMMAND(ID_PICT_TRIM,OnPictTrim)
	ON_UPDATE_COMMAND_UI(ID_COLOR_CHANGE_LINE,	OnUpdateLineColor)
	ON_UPDATE_COMMAND_UI(ID_INVERT_H,       OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_INVERT_V,       OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PE_AVERAGE,     OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PE_MEDIAN,      OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PE_MOSAIC,      OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PE_OILPAINT,    OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PE_POSTERIZE,   OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PE_SHARPNESS,   OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PE_SOLARIZE,    OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PICT_DRAW_DOT,  OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PICT_EDIT,      OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PICT_EDITVIEW,OnUpdateEditview)
	ON_UPDATE_COMMAND_UI(ID_PICT_MOUSE_TRIM,OnUpdatePictMouseTrim)
	ON_UPDATE_COMMAND_UI(ID_PICT_PREVIEW,OnUpdatePreview)
	ON_UPDATE_COMMAND_UI(ID_PICT_RELOAD,    OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PICT_RESET_SELRECT,OnUpdateResetSelRect)
	ON_UPDATE_COMMAND_UI(ID_PICT_RESIZE,    OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PICT_SEL_RECT,  OnUpdateEditableCmd)
	ON_UPDATE_COMMAND_UI(ID_PICT_TRIM,      OnUpdateEditableCmd)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

BOOL PictCharView::s_bPreview = FALSE;

IMPLEMENT_DYNCREATE(PictCharView,ScrollViewEx)
PictCharView::PictCharView()
:	m_crLine(RGB(0x00,0x00,0x00)),
	m_crFill(RGB(0xFF,0xFF,0xFF))
{
	ScrollViewEx::Center(TRUE);
	m_rcSel.SetRectEmpty();
}

PictCharView::~PictCharView() {
}

BOOL PictCharView::IsEditable() const {
	return GetPictChar()->IsEditable();
}

void PictCharView::OnUpdateEditableCmd(CCmdUI* cmd) {
	cmd->Enable(IsEditable());
}

void PictCharView::AdjustSelRect() {
	m_rcSel.NormalizeRect();
	if (m_rcSel.IsRectEmpty() || m_rcSel.IsRectNull())
		return;

	CRect rc;
	GetPictBmpRect(rc);
	
	m_rcSel.IntersectRect(m_rcSel,rc);
}

PictChar* PictCharView::GetPictChar() {
	return GetCharT();
}

const PictChar* PictCharView::GetPictChar() const {
	return GetCharT();
}

SpicydogDoc* PictCharView::GetDocument() {
	return DYNAMIC_DOWNCAST(SpicydogDoc,CView::GetDocument());
}

const SpicydogDoc* PictCharView::GetDocument() const {
	return DYNAMIC_DOWNCAST(SpicydogDoc,CView::GetDocument());
}

void PictCharView::OnInitialUpdate() {
	ScrollViewEx::OnInitialUpdate();
	
	CRect rc;
	GetPictChar()->GetRect(rc);
	ScrollViewEx::SetScrollSizes(rc.Size(),1,CPoint(0,0));
	ScrollViewEx::SetScale(1);

	UpdateCommandModes();
	ActivateDefaultControler();
}

void PictCharView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) {	
	ScrollViewEx::OnUpdate(pSender,lHint,pHint);

	CRect rc;
	GetPictChar()->GetRect(rc);
	ScrollViewEx::SetScrollSizes(rc.Size(),1,CPoint(0,0));

	AdjustSelRect();
	Invalidate();
}

void PictCharView::OnDraw(CDC* pDC) {
	ScrollViewEx::FillBkgnd(pDC,&CBrush(GetSysColor(COLOR_BTNFACE)));
	ScrollViewEx::FillScrollabeRect(pDC,&CBrush(GetSysColor(GetDocument()->appli->appliConf->bkgndColor)));

	if (s_bPreview) {
		CRect rc;
		ScrollViewEx::GetDocumentRect(rc);
		CSetDCtoDP setDCtoLP(pDC);
		FillNullRect(pDC,rc);
	}

	DrawChar(pDC);
	TWndControlerMgr<ScrollViewEx>::OnDraw(pDC);
	DrawSelRect(pDC);
	DrawResizeHandle(pDC);
	
	CPoint ptDP = GetPictChar()->GetCenter();
	LPtoDP(&ptDP);
	DrawCenter(pDC,ptDP);

	if (ACTIVE_PREF.DisplayPixelGrid && 4 < GetScale()) {
		CPen pen(PS_DOT,0,ACTIVE_PREF.PixelGridColor);
		ScrollViewEx::DrawGridHelper(pDC,pen,1);	
	}
	if (ACTIVE_PREF.DisplayGrid && 0.4 < GetScale()) {
		CPen pen(PS_DOT,0,ACTIVE_PREF.GridInterval);
		ScrollViewEx::DrawGridHelper(pDC,pen,ACTIVE_PREF.GridInterval);	
	}
}

int PictCharView::HitResizeHandle(CPoint ptWnd) const {
	CRect rc;
	GetPictBmpRect(rc);
	LPtoDP(&rc);

	for (int i = 0; i < 8; ++i) {
		CRect rcHandle;
		GetHandleRect(rc,i,rcHandle,4,CRectTracker::resizeOutside);
		if (rcHandle.PtInRect(ptWnd)) {
			return i;
		}
	}

	return CRectTracker::hitMiddle;
}

void PictCharView::DrawChar(CDC* pDC) const {
	const PictChar* chr = GetPictChar();
	if (s_bPreview) {
		AffineMatrix matrix;
		Coloring coloring;
		const CPoint ptCenter = chr->GetCenter();
		matrix.x = ptCenter.x;
		matrix.y = ptCenter.y;
		chr->Draw(pDC,&matrix,&coloring);
	} else {
		chr->DrawEditview(pDC);
	}
}

void PictCharView::GetPictBmpRect(CRect& rcLP) const {
	GetPictChar()->GetRect(rcLP);
	rcLP.OffsetRect(-rcLP.TopLeft());
}

void PictCharView::DrawResizeHandle(CDC* pDC) const {
	CRect rc;
	GetPictBmpRect(rc);
	LPtoDP(&rc);

	int nSavedDC = pDC->SaveDC();
	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

	for (int i = 0; i < 8; ++i) {
		CRect rcHandle;
		GetHandleRect(rc,i,rcHandle,6,CRectTracker::resizeOutside);
		pDC->FillSolidRect(rcHandle, RGB(0, 0, 0));
		rcHandle.DeflateRect(1,1);
		pDC->FillSolidRect(rcHandle, RGB(0xFF,0xFF,0xFF));
	}

	pDC->RestoreDC(nSavedDC);
}

void PictCharView::DrawSelRect(CDC* pdc) const {
	if (m_rcSel.IsRectEmpty())
		return;
	CRect rcDP(m_rcSel);
	LPtoDP(rcDP);

	pdc->DrawDragRect(rcDP,CSize(1,1),NULL,CSize(1,1));
}

void PictCharView::OnPictResize() {
	if (!IsEditable()) {
		ASSERT(FALSE);
		return;
	}

	PictChar* pChar = GetPictChar();
	PictResizeDlg dlg(pChar->GetPictData().m_bmp,this);
	if (dlg.DoModal() == IDOK) {
		CSize szNew = dlg.GetNewSize();;
		PictData data(pChar->GetPictData());
		if (!LBitmap_Resize(pChar->GetPictData().m_bmp,data.m_bmp,szNew.cx,szNew.cy)) {
			ASSERT(FALSE);
			AfxThrowMemoryException();
		}
		GetDocument()->DoCmd(new PictModDataCmd(IDCMD_PICT_RESIZE,GetPictChar(),data));
		GetDocument()->UpdateAll(NULL);
	}
}

/**
	どの辺を基底にトリミングするかに応じて
	実際のトリミング矩形を求める。
*/
void TrimAlignToOffsetHelper(int& sz,int& dst,int& src,int nAlign,int szNew,int szOld) {
	ASSERT(0 <= nAlign && nAlign <= 2);

	if (szNew == szOld)
	{
		sz = szNew;
		dst = 0;
		src = 0;
	}
	else if (szNew < szOld)
	{
		sz = szNew;
		dst = 0;
		switch (nAlign)
		{
		case 0:
			src = 0;
			break;
		case 1:
			src = (szOld - szNew)/2;
			ASSERT(0 < src && src < szOld);
			break;
		case 2:
			src = szOld - szNew;
			ASSERT(0 < src && src < szOld);
			break;
		default:
			ASSERT(FALSE);
		}

	} else {
		ASSERT(szOld < szNew);
		sz = szOld;
		src = 0;
		switch (nAlign) {
		case 0:
			dst = 0;
			break;
		case 1:
			dst = (szNew - szOld)/2;
			ASSERT(0 < dst);
			break;
		case 2:
			dst = szNew - szOld;
			ASSERT(0 < dst);
			break;
		default:
			ASSERT(FALSE);
		}
	}
}

void TrimAlignToOffset
(
	int& szX,
	int& szY,
	int& xDst,
	int& yDst,
	int& xSrc,
	int& ySrc,
	int nNewW,
	int nNewH,
	int nAlign,
	int nOldW,
	int nOldH
)
{
	{
		int nAlignY = 0;
		switch (nAlign)
		{
		case 0:
		case 1:
		case 2:
			nAlignY = 0;
			break;
		case 3:
		case 4:
		case 5:
			nAlignY = 1;
			break;
		case 6:
		case 7:
		case 8:
			nAlignY = 2;
			break;
		default:
			ASSERT(FALSE);
		}
		TrimAlignToOffsetHelper(szY,yDst,ySrc,nAlignY,nNewH,nOldH);
	}

	{
		int nAlignX = 0;
		switch (nAlign)
		{
		case 0:
		case 3:
		case 6:
			nAlignX = 0;
			break;
		case 1:
		case 4:
		case 7:
			nAlignX = 1;
			break;
		case 2:
		case 5:
		case 8:
			nAlignX = 2;
			break;
		default:
			ASSERT(FALSE);
		}
		TrimAlignToOffsetHelper(szX,xDst,xSrc,nAlignX,nNewW,nOldW);
	}
}


/**
	画像トリミングダイアログを開く
*/
void PictCharView::OnPictTrim() {
	if (!IsEditable())
		{ASSERT(FALSE);return;}

	PictChar* pChar = GetPictChar();
	const PictData& data  = pChar->GetPictData();
	const LBitmapBase& bmp = data.m_bmp;

	int nOrgW = LBitmap_GetWidth(bmp);
	int nOrgH = LBitmap_GetHeight(bmp);
	PictTrimDlg dlg(nOrgW,nOrgH,this);
	if (dlg.DoModal() == IDOK)
	{
		int nW;
		int nH;
		int nAlign;
		dlg.GetTrimParam(nW,nH,nAlign);

		int nNewW;
		int nNewH;
		int xSrc;
		int ySrc;
		int xDst;
		int yDst;
		TrimAlignToOffset(nNewW,nNewH,xDst,yDst,xSrc,ySrc,nW,nH,nAlign,nOrgW,nOrgH);

		PictData dataNew;
		LBitmapBase bmpNew;
		if (LBitmap_Resize(data.m_bmp,dataNew.m_bmp,nW,nH) != SUCCESS) {
			DEBUG_ONLY(bmpNew.DisplayErrorList(m_hWnd));
			AfxThrowMemoryException();		
		}

		
		{
			COLORREF cr = GetDocument()->appli->appliConf->bkgndColor;
			if (dataNew.m_bmp.Fill(cr) != SUCCESS) {
				DEBUG_ONLY(dataNew.m_bmp.DisplayErrorList(m_hWnd));
				AfxThrowMemoryException();
			}
		}

		ASSERT(dataNew.m_bmp.GetBitsPerPixel() == const_cast<LBitmapBase&>(bmp).GetBitsPerPixel());
		if (dataNew.m_bmp.Combine(const_cast<LBitmapBase*>(&bmp),nNewW,nNewH,xDst,yDst,xSrc,ySrc) != SUCCESS) {
			DEBUG_ONLY(dataNew.m_bmp.DisplayErrorList(m_hWnd));
			AfxThrowMemoryException();
		}

		GetDocument()->DoCmd(new PictModDataCmd(IDCMD_PICT_RESIZE,GetPictChar(),dataNew));
		GetDocument()->UpdateAll(NULL);
	}
}

void PictCharView::OnPictMouseTrim() {
	if (!IsEditable())
		{ASSERT(FALSE);return;}

	if (m_rcSel.IsRectEmpty()) {
		::AfxMessageBox(IDCS_ERR_PICT_MOUSE_TRIM_SEL_EMPTY);
		return;
	}

	const PictData&   data = GetPictChar()->GetPictData();
	const LBitmapBase& bmp  = data.m_bmp;

	PictData dataNew;
	if (!LBitmap_Copy(bmp,dataNew.m_bmp))
		AfxThrowMemoryException();

	if (dataNew.m_bmp.Trim(m_rcSel.left,m_rcSel.top,m_rcSel.Width(),m_rcSel.Height()) != SUCCESS)
		AfxThrowMemoryException();
	
	GetDocument()->DoCmd(new PictModDataCmd(IDCMD_PICT_TRIM,GetPictChar(),dataNew));
	GetDocument()->UpdateAll(NULL);

	m_rcSel.SetRectEmpty();

	ASSERT_VALID(this);
}

void PictCharView::OnUpdatePictMouseTrim(CCmdUI* pcmd) {
	pcmd->Enable(!m_rcSel.IsRectEmpty() && IsEditable());
}

COLORREF PictCharView::GetLineColor() const {
	ASSERT(m_crLine <= 0xFFFFFF || m_crLine == 0xFFFFFFFF);
	return m_crLine;
}

COLORREF PictCharView::GetFillColor() const {
	ASSERT(m_crFill <= 0xFFFFFF || m_crFill == 0xFFFFFFFF);
	return m_crFill;
}

void PictCharView::OnLineColorChanged() {
	COLORREF cr = ColorBoxEx::GetCurrentColor();
	if (cr != NULLCOLOR) {
		m_crLine = cr;
	}
}

void PictCharView::OnUpdateLineColor(CCmdUI* pCmdUI) {
	ColorBoxEx::OnUpdateCmdUI(pCmdUI,GetLineColor());
}

void PictCharView::UpdateCommandModes() {
	AppendControler(ID_TOOL_SELECT,  new SelPictViewCnt);
	AppendControler(ID_PICT_SEL_RECT,new SelRectPictViewCnt);
	AppendControler(ID_TOOL_CENTER,  new CenterPictViewCnt);
	AppendControler(ID_PICT_DRAW_DOT,new DotPictViewCnt);
	AppendControler(ID_SCRVIEW_ZOOM, new ScrollViewZoomCnt);
	AppendControler(ID_SCRVIEW_MOVE, new ScrollViewMoveCnt);
	SetDefaultControlerID(ID_TOOL_SELECT);
}

BOOL CALLBACK PE_SHARPNESS(pBITMAPHANDLE pbmp,int nParam) {
	return L_SharpenBitmap(pbmp,nParam) == SUCCESS;
}

BOOL CALLBACK PE_MOSAIC(pBITMAPHANDLE pbmp,int nParam) {
	return L_MosaicBitmap(pbmp,nParam) == SUCCESS;
}

BOOL CALLBACK PE_AVERAGE(pBITMAPHANDLE pbmp,int nParam) {
	return L_AverageFilterBitmap (pbmp,nParam) == SUCCESS;
}

BOOL CALLBACK PE_MEDIAN(pBITMAPHANDLE pbmp,int nParam) {
	return L_MedianFilterBitmap (pbmp,nParam) == SUCCESS;
}

BOOL CALLBACK PE_SOLARIZE(pBITMAPHANDLE pbmp,int nParam) {
	return L_SolarizeBitmap (pbmp,nParam) == SUCCESS;
}

BOOL CALLBACK PE_OILIFY(pBITMAPHANDLE pbmp,int nParam) {
	return L_OilifyBitmap (pbmp,nParam) == SUCCESS;
}

BOOL CALLBACK PE_POSTERIZE(pBITMAPHANDLE pbmp,int nParam) {
	return L_PosterizeBitmap (pbmp,nParam) == SUCCESS;
}

void PictCharView::OnPEMosaic() {
	DoEffect(IDS_PENAME_MOSAIC,IDS_PEPARAM_MOSAIC,IDS_PEUNIT_MOSAIC,1,16,3,PE_MOSAIC);
}

void PictCharView::OnPESharpness() {
	DoEffect(IDS_PENAME_SHARPNESS,IDS_PEPARAM_SHARPNESS,IDS_PEUNIT_SHARPNESS,-1000,1000,0,PE_SHARPNESS);
}

void PictCharView::OnPEAverage() {
	DoEffect(IDS_PENAME_AVERAGE,IDS_PEPARAM_AVERAGE,IDS_PEUNIT_AVERAGE,1,16,3,PE_AVERAGE);
}

void PictCharView::OnPEMedian() {
	DoEffect(IDS_PENAME_MEDIAN,IDS_PEPARAM_MEDIAN,IDS_PEUNIT_MEDIAN,1,16,3,PE_MEDIAN);
}

void PictCharView::OnPESolarize() {
	DoEffect(IDS_PENAME_SOLARIZE,IDS_PEPARAM_SOLARIZE,IDS_PEUNIT_SOLARIZE,0,255,0,PE_SOLARIZE);
}

void PictCharView::OnPEOilify() {
	DoEffect(IDS_PENAME_OILIFY,IDS_PEPARAM_OILIFY,IDS_PEUNIT_OILIFY,1,16,3,PE_OILIFY);
}

void PictCharView::OnPEPosterize() {
	DoEffect(IDS_PENAME_POSTERIZE,IDS_PEPARAM_POSTERIZE,IDS_PEUNIT_POSTERIZE,2,64,2,PE_POSTERIZE);
}

void PictCharView::DoEffect
(
	UINT nEffectNameID,
	UINT nParamNameID,
	UINT nUnitNameID,
	int nMin,
	int nMax,
	int nDefault,
	PICT_EFFECT_PROC proc
) {
	GetDocument()->BeginEdit();
	if (!IsEditable()) {
		ASSERT(FALSE);
		return;
	}
	
	const PictData& data = GetPictChar()->GetPictData();
	PictEffectPictDlg dlg(data.m_bmp,nEffectNameID,nParamNameID,nUnitNameID,nMin,nMax,nDefault,proc);
	if (dlg.DoModal() == IDOK) {
		PictData dataNew;

		if (dataNew.m_bmp.Copy(const_cast<LBitmapBase&>(data.m_bmp)) != SUCCESS)
			AfxThrowMemoryException();
		
		int nParam = dlg.GetEffectParam();
		if (!(*proc)(dataNew.m_bmp.GetHandle(),nParam))
			AfxThrowMemoryException();

		Cmd* cmd = new PictModDataCmd(IDCMD_PICT_TRIM,GetPictChar(),dataNew);
		CString strEffectName = FormatString(nEffectNameID);
		cmd->SetCmdName(-1,strEffectName);
		GetDocument()->DoCmd(cmd);
		GetDocument()->UpdateAll(NULL);
	}
}

void PictCharView::OnInvertH() {
	GetDocument()->BeginEdit();
	if (!IsEditable()) {
		ASSERT(FALSE);
	}
	PictData dataNew;
	if (dataNew.m_bmp.Copy(const_cast<LBitmapBase&>(GetPictChar()->GetPictData().m_bmp)) != SUCCESS)
		AfxThrowMemoryException();

	dataNew.m_bmp.Reverse();
	GetDocument()->DoCmd(new PictModDataCmd(IDCMD_PICT_INVERT_H,GetPictChar(),dataNew));
	GetDocument()->UpdateAll(NULL);
}

void PictCharView::OnInvertV() {
	GetDocument()->BeginEdit();
	if (!IsEditable()) {
		ASSERT(FALSE);
	}
	PictData dataNew;
	if (dataNew.m_bmp.Copy(const_cast<LBitmapBase&>(GetPictChar()->GetPictData().m_bmp)) != SUCCESS)
		AfxThrowMemoryException();

	dataNew.m_bmp.Flip();
	GetDocument()->DoCmd(new PictModDataCmd(IDCMD_PICT_INVERT_V,GetPictChar(),dataNew));
	GetDocument()->UpdateAll(NULL);
}

void PictCharView::OnEditByExe() {
	GetDocument()->BeginEdit();
	if (!IsEditable()) {
		ASSERT(FALSE);
	}
	PictData dataNew;
	if (dataNew.m_bmp.Copy(const_cast<LBitmapBase&>(GetPictChar()->GetPictData().m_bmp)) != SUCCESS)
		AfxThrowMemoryException();
	
	if (PictChar::EditByExe(dataNew)) {
		GetDocument()->DoCmd(new PictModDataCmd(IDCMD_PICT_EDIT_BY_EXE,GetPictChar(),dataNew));
		GetDocument()->UpdateAll(NULL);
	}
}

void PictCharView::OnReload() {
	GetDocument()->BeginEdit();
	if (!IsEditable()) {
		ASSERT(FALSE);
	}

	CString strFilter;
	VERIFY(strFilter.LoadString(IDS_PICT_FILTER));
	CFileDialog dlg(TRUE,NULL,NULL,0,strFilter,this);
	if (dlg.DoModal() == IDOK) {
		PictData dataNew;
		if (PictChar::LoadBmp(dlg.GetPathName(),dataNew)) {
			GetDocument()->DoCmd(new PictModDataCmd(IDCMD_PICT_RELOAD,GetPictChar(),dataNew));
			GetDocument()->UpdateAll(NULL);
		}
	}
}

void PictCharView::OnPreview() {
	s_bPreview = TRUE;
	Invalidate();
}

void PictCharView::OnEditview() {
	s_bPreview = FALSE;
	Invalidate();
}

void PictCharView::OnUpdatePreview(CCmdUI* p) {
	p->SetRadio(s_bPreview);
}

void PictCharView::OnUpdateEditview(CCmdUI* p) {
	p->SetRadio(!s_bPreview);
}

void PictCharView::OutputPreview2File()
{
	CFileDialog dlg(FALSE,
					_T("gif"),
					GetPictChar()->GetName() + _T(".gif"),
					0,
					_T("Gif Files (*.gif)|*.gif||"),
					this);
	if (dlg.DoModal() != IDOK)
		return;
	
	GetPictChar()->ExportCache(dlg.GetPathName());
}

void PictCharView::OnPictOpt() {
	PictChar* pPictChar = GetPictChar();
	EditPictQuantizeDlg dlg(pPictChar);
	if (dlg.DoModal() == IDOK) {
		GetDocument()->DoCmd(new PictSetOptParamCmd(pPictChar,dlg.GetQuantizeParam()));
		GetDocument()->UpdateAll(NULL);
	}
}

void PictCharView::OnResetSelRect() {
	ResetSelRect();
}

void PictCharView::OnUpdateResetSelRect(CCmdUI* cmd) {
	cmd->Enable(!m_rcSel.IsRectEmpty());
}

void PictCharView::ResetSelRect() {
	m_rcSel.SetRectEmpty();
	Invalidate();
}

void PictCharView::GetSelRect(CRect& rc) const {
	ASSERT_VALID(this);
	rc = m_rcSel;
}

void PictCharView::SetSelRect(const CRect& rc) {
	m_rcSel = rc;
	AdjustSelRect();
	Invalidate();
	ASSERT_VALID(this);
}