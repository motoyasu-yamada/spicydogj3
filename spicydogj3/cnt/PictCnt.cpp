#include "STDAFX.H"
#include "RESOURCE.H"
#include "PictCnt.H"

#include "SpicydogDoc.H"
#include "model/char/PictChar.H"
#include "view/PictCharView.H"
#include "cmd/char/PictCharCmd.h"
#include "utility/WndUtility.h"
#include "utility/LeadToolUtility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(PictViewCnt,CCmdTarget)
	//{{AFX_MSG_MAP(PictViewCnt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

PictViewCnt::PictViewCnt(){
}

const PictChar* PictViewCnt::GetPictChar() const {
	return GetViewT()->GetCharT();
}

PictChar* PictViewCnt::GetPictChar() {
	return GetViewT()->GetCharT();
}

LRESULT PictViewCnt::Default() {
	return GetViewT()->Default();
}

BOOL PictViewCnt::IsEditable() const {
	return GetViewT()->IsEditable();
}

BEGIN_MESSAGE_MAP(SelPictViewCnt,PictViewCnt)
	//{{AFX_MSG_MAP(SelPictViewCnt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

SelPictViewCnt::SelPictViewCnt()
:	PictViewCnt() {
	m_hcur = ::LoadCursor(NULL,IDC_ARROW);
		m_tracker.m_nHandleSize = 4;
		m_tracker.m_nStyle = CRectTracker::solidLine | CRectTracker::resizeOutside;
}

BOOL SelPictViewCnt::OnSetCursor(CWnd*,UINT nHit,UINT) {
	if (!IsEditable()) {
		return Default();
	}

	if (nHit == HTCLIENT) {
		GetViewT()->GetPictBmpRect(m_tracker.m_rect);
		GetViewT()->LPtoDP(m_tracker.m_rect);
		if (!m_tracker.SetCursor(GetViewT(),HTCLIENT)) {
			SetCursor(m_hcur);
		}
		return TRUE;
	}
	else {
		return Default();
	}
}

void SelPictViewCnt::OnRButtonDown(UINT,CPoint) {
	TrackNewPopupMenu(GetViewT()->GetTopLevelFrame(),IDM_PICTVIEW,0);
}

void SelPictViewCnt::OnLButtonDown(UINT n,CPoint pt) {
	if (!IsEditable()) {
		Default();
		return;
	}

	GetViewT()->GetPictBmpRect(m_tracker.m_rect);
	GetViewT()->LPtoDP(m_tracker.m_rect);
	
	int nHitTest = m_tracker.HitTest(pt);
	if (nHitTest == CRectTracker::hitNothing ||
        nHitTest == CRectTracker::hitMiddle )
		return;

	if (!m_tracker.TrackResize(GetViewT(),pt))
		return;

	CRect rc = m_tracker.m_rect;		
	GetViewT()->DPtoLP(rc);
	CSize szNew = rc.Size();
	szNew.cx = max(min(szNew.cx,PICT_MAX_WIDTH), PICT_MIN_WIDTH);
	szNew.cy = max(min(szNew.cy,PICT_MAX_HEIGHT),PICT_MIN_HEIGHT);

	PictData dataNew(GetPictChar()->GetPictData());
	if (!LBitmap_Resize(GetPictChar()->GetPictData().m_bmp,dataNew.m_bmp,szNew.cx,szNew.cy)) {
		AfxThrowMemoryException();
	}
	GetDocumentT()->DoCmd(new PictModDataCmd(IDCMD_PICT_RESIZE,GetPictChar(),dataNew));
	GetDocumentT()->UpdateAll(NULL);
}

BEGIN_MESSAGE_MAP(SelRectPictViewCnt,PictViewCnt)
	//{{AFX_MSG_MAP(SelRectPictViewCnt)
	ON_UPDATE_COMMAND_UI(ID_PICT_SEL_RECT,OnUpdateCmdUI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HCURSOR SelRectPictViewCnt::s_hcurReset;
HCURSOR  SelRectPictViewCnt::s_hcurSel;
void SelRectPictViewCnt::clinit() {
	static BOOL clinited = FALSE;
	if (clinited) {
		return;
	}

	s_hcurReset = AfxGetApp()->LoadCursor(IDC_SEL_RECT_RESET);
	s_hcurSel   = AfxGetApp()->LoadCursor(IDC_SEL_RECT);
	clinited = TRUE;
}

SelRectPictViewCnt::SelRectPictViewCnt()
:	PictViewCnt() {
	clinit();
	m_tracker.m_nStyle = CRectTracker::dottedLine;
}

void SelRectPictViewCnt::OnEnter() {
	if (!IsEditable()) {
		RejectEnter();
	}
}

void SelRectPictViewCnt::OnUpdateCmdUI(CCmdUI* pCmdUI) {
	pCmdUI->Enable(IsEditable());
	if (IsEditable()) {
		pCmdUI->SetCheck((GetCntMgr()->GetActiveControler() == this) ? 1 : 0);
	} else {
		pCmdUI->SetCheck(0);
	}
}

BOOL SelRectPictViewCnt::OnSetCursor(CWnd*,UINT nHit,UINT) {
	ASSERT(IsEditable());
	if (nHit == HTCLIENT) {
		CPoint pt;
		VERIFY(GetCursorPos(&pt));
		GetViewT()->ScreenToClient(&pt);
		if (GetViewT()->PtInDocumentRect(pt)) {
			SetCursor(s_hcurSel);
		} else {
			SetCursor(s_hcurReset);
		}
		return TRUE;
	} else
		return Default();
}

void SelRectPictViewCnt::OnRButtonDown(UINT,CPoint) {
	ASSERT(IsEditable());
	GetCntMgr()->ActivateDefaultControler();
}

void SelRectPictViewCnt::OnLButtonDown(UINT n,CPoint pt) {
	ASSERT(IsEditable());
	int nHit = GetViewT()->HitResizeHandle(pt); 
	if (nHit == CRectTracker::hitMiddle) {
		if (m_tracker.TrackRubberBand(GetViewT(),pt)) {
			CRect rc = m_tracker.m_rect;
			
			GetViewT()->DPtoLP(rc);
			GetViewT()->SetSelRect(rc);
		}
	} else if (nHit == CRectTracker::hitNothing){
		GetViewT()->ResetSelRect();
	}
}

BEGIN_MESSAGE_MAP(CenterPictViewCnt,PictViewCnt)
	//{{AFX_MSG_MAP(CenterPictViewCnt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CenterPictViewCnt::CenterPictViewCnt()
:	PictViewCnt() {
	m_hcur = ::LoadCursor(NULL,IDC_ARROW);
	m_hcurCenter = AfxGetApp()->LoadCursor(IDC_CENTER);
}

BOOL CenterPictViewCnt::OnSetCursor(CWnd*,UINT nHit,UINT)
{
	if (nHit == HTCLIENT) {
		CPoint pt;
		VERIFY(GetCursorPos(&pt));
		GetViewT()->ScreenToClient(&pt);
		if (GetViewT()->PtInDocumentRect(pt))
			SetCursor(m_hcurCenter);
		else
			SetCursor(m_hcur);
		
		return TRUE;
	} else {
		return Default();
	}
}

void CenterPictViewCnt::OnRButtonDown(UINT,CPoint) {
	GetCntMgr()->ActivateDefaultControler();
}

void CenterPictViewCnt::OnLButtonDown(UINT n,CPoint ptDP) {
	if (GetViewT()->PtInDocumentRect(ptDP)) {
		CPoint ptLP = ptDP;
		GetViewT()->DPtoLP(&ptLP);
		GetDocumentT()->DoCmd(new PictSetCenterCmd(GetPictChar(),ptLP));
		GetDocumentT()->UpdateAll(NULL);
	}

	GetCntMgr()->ActivateDefaultControler();
}


BEGIN_MESSAGE_MAP(DotPictViewCnt,PictViewCnt)
	//{{AFX_MSG_MAP(DotPictViewCnt)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_PICT_DRAW_DOT,OnUpdateCmdUI)
END_MESSAGE_MAP()

DotPictViewCnt::DotPictViewCnt()
:	PictViewCnt(),
	m_bDown(FALSE) {
	m_hcur = AfxGetApp()->LoadCursor(IDC_DRAW_DOT);
	m_hcurNo = ::LoadCursor(NULL,IDC_NO);
}

void DotPictViewCnt::OnEnter() {
	if (!IsEditable()) {
		RejectEnter();
		return;
	}
	BeginDraw();
}

void DotPictViewCnt::OnExit() {
	ASSERT(IsEditable());
	EndDraw();
}

void DotPictViewCnt::BeginDraw() {
	ASSERT(IsEditable());
	if (m_drawdots.size() != 0) {
		ASSERT(FALSE);
		m_drawdots.clear();
	}
	ASSERT(m_drawdots.size() == 0);
}

void DotPictViewCnt::EndDraw() {
	ASSERT(IsEditable());
	if (0 == m_drawdots.size()) {
		return;
	}

	PictData dataNew(GetPictChar()->GetPictData());
	std::list<DRAWDOT>::const_iterator ci;
	for (ci = m_drawdots.begin();ci != m_drawdots.end();ci++) {
		const DRAWDOT& dd = *ci;
		VERIFY(dataNew.m_bmp.PutPixelColor(dd.m_pt.y,dd.m_pt.x,dd.m_cr) == SUCCESS);
	}

	GetDocumentT()->DoCmd(new PictModDataCmd(IDCMD_PICT_DRAW_DOT,GetPictChar(),dataNew));
	GetDocumentT()->UpdateAll(NULL);

	m_drawdots.clear();
}

void DotPictViewCnt::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	ASSERT(IsEditable());
	switch (nChar) {
	case VK_ESCAPE:
		GetCntMgr()->ActivateDefaultControler();
		break;
	}
}

void DotPictViewCnt::OnDraw(CDC* pDC) {
	ASSERT(IsEditable());
	
	int nSaved = pDC->SaveDC();
	std::list<DRAWDOT>::const_iterator ci;
	for (ci = m_drawdots.begin();ci != m_drawdots.end();ci++) {
		const DRAWDOT& dd = *ci;
		CRect rc(dd.m_pt,CSize(1,1));
		pDC->FillSolidRect(rc,dd.m_cr);
	}
	VERIFY(pDC->RestoreDC(nSaved));
}

/*
int cutdown_double(double d) {
	return d;
}

CPoint cutdown_double(FPoint fpt) {
	return CPoint(cutdown_double(fpt.x),cutdown_double(fpt.y));
}
*/

BOOL DotPictViewCnt::OnSetCursor(CWnd*,UINT nHit,UINT) {
	ASSERT(IsEditable());
	if (nHit == HTCLIENT) {
		CPoint pt;
		VERIFY(GetCursorPos(&pt));
		PictCharView* pView = GetViewT();
		ASSERT_VALID(pView);
		pView->ScreenToClient(&pt);
		pView->DPtoLP(&pt);
		
		CRect rcCanDraw;
		pView->GetPictBmpRect(rcCanDraw);

		BOOL bCanDot = rcCanDraw.PtInRect(pt);
		SetCursor(bCanDot ? m_hcur : m_hcurNo);
		return TRUE;
	} else {
		return Default();
	}
}

void DotPictViewCnt::Draw(CPoint ptDP) {
	ASSERT(IsEditable());
	COLORREF cr = GetViewT()->GetLineColor();
	CPoint ptLP = ptDP;
	GetViewT()->DPtoLP(&ptLP);
	if (m_ptDocLastDot == ptLP) {
		return;
	}

	CPoint pt1;
	CPoint pt2;
	if (m_ptDocLastDot.x < ptLP.x) {
		pt1 = m_ptDocLastDot;
		pt2 = ptLP;
	} else {
		pt2 = m_ptDocLastDot;
		pt1 = ptLP;
	}
	double x1 = pt1.x;
	double y1 = pt1.y;
	double x2 = pt2.x;
	double y2 = pt2.y;
	double delta_y = y2 - y1;
	double delta_x = x2 - x1;
	double x = x1;
	double y = y1;
	
	ASSERT(x1 <= x2);
	if (delta_x == 0) {
		for(;;) {
			DrawDot(CPoint(x,y),cr);

			if (delta_y < 0) {
				y --;
				if (y < y2)
					break;
			} else {
				y ++;
				if (y2 < y)
					break;
			}
		}
	} else {
		double add = delta_y / delta_x;
		for(double x = x1;x <= pt2.x;x++) {
			DrawDot(CPoint(x,y),cr);
			y += add;
		}
	}

	m_ptDocLastDot = ptLP;
	GetViewT()->Invalidate();
}

void DotPictViewCnt::DrawDot(CPoint ptDoc,COLORREF cr) {
	ASSERT(IsEditable());
	CRect rc;
	GetViewT()->GetPictBmpRect(rc);
	if (rc.PtInRect(ptDoc)) {
		COLORREF crPix = cr;
		DRAWDOT dd;
		dd.m_pt = ptDoc;
		dd.m_cr = cr;
		m_drawdots.push_back(dd);
	}
}

void DotPictViewCnt::OnMouseMove(UINT nFlags,CPoint pt) {
	ASSERT(IsEditable());
	if (nFlags && MK_LBUTTON) {
		if (m_bDown)
			Draw(pt);
	}
}

void DotPictViewCnt::OnLButtonDown(UINT,CPoint ptDP) {
	ASSERT(IsEditable());
	ASSERT(!m_bDown);
	if (m_bDown) {
		EndDraw();
		BeginDraw();
		m_bDown = FALSE;
	}

	m_bDown = TRUE;
	m_ptDocLastDot = ptDP;
	GetViewT()->DPtoLP(&m_ptDocLastDot);
	DrawDot(m_ptDocLastDot,GetViewT()->GetLineColor());
}

void DotPictViewCnt::OnLButtonUp(UINT,CPoint) {
	ASSERT(IsEditable());
	EndDraw();
	BeginDraw();
	m_bDown = FALSE;
}

void DotPictViewCnt::OnUpdateCmdUI(CCmdUI* pCmdUI) {
	pCmdUI->Enable(IsEditable());
	if (IsEditable()) {
		pCmdUI->SetCheck((GetCntMgr()->GetActiveControler() == this) ? 1 : 0);
	} else {
		pCmdUI->SetCheck(0);
	}
}
