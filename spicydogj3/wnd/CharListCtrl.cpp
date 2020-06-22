#include "stdafx.h"

#include "CharListCtrl.h"
#include "dlg/char/CharListDlg.H"
#include "utility/WndUtility.h"
#include "SpicydogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CharListCtrlDropTarget::CharListCtrlDropTarget()
:	m_pDictionaryView(NULL) {
}

void CharListCtrlDropTarget::Register(CWnd* pwnd,CharListDlg* pDic)
{
	ASSERT(m_pDictionaryView == NULL);
	m_pDictionaryView = pDic;
	ASSERT_VALID(m_pDictionaryView);
	VERIFY(COleDropTarget::Register(pwnd));
}

DROPEFFECT CharListCtrlDropTarget::OnDragEnter(CWnd* pWnd,COleDataObject* pObj,DWORD dw,CPoint) {
	if (pObj->IsDataAvailable(CF_CHARS))
		return (dw & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;

	return DROPEFFECT_NONE;
}

DROPEFFECT CharListCtrlDropTarget::OnDragOver(CWnd* pWnd,COleDataObject* pObj,DWORD dw,CPoint pt)
{
	return OnDragEnter(pWnd,pObj,dw,pt);
}

BOOL CharListCtrlDropTarget::OnDrop(CWnd*,COleDataObject* pObj,DROPEFFECT de,CPoint pt)
{
	if (de == DROPEFFECT_COPY || de == DROPEFFECT_MOVE)
		return m_pDictionaryView->AcceptOle(pObj,TRUE,de,pt);
	return FALSE;
}

BEGIN_MESSAGE_MAP(CharListCtrl, CListCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CharListCtrl::CharListCtrl()
:	m_pDictionaryView(NULL) {
}

CharListCtrl::~CharListCtrl() {
	ASSERT(m_pDictionaryView != NULL);
}

void CharListCtrl::Register(CharListDlg* p) {
	ASSERT(m_pDictionaryView == NULL);
	m_pDictionaryView = p;
	ASSERT_VALID(m_pDictionaryView);
	m_Target.Register(this,m_pDictionaryView);
}

void CharListCtrl::OnLButtonDown(UINT nFlags, CPoint point) {
	CListCtrl::OnLButtonDown(nFlags, point);
	if (IsToStartDrag(this,point,TRUE)) {
		ASSERT_VALID(m_pDictionaryView);
		ASSERT_VALID(&m_Source);
		if (m_pDictionaryView->SubmitOle(&m_Source,TRUE)) {
			DROPEFFECT de = m_Source.DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE);
			ASSERT(de == DROPEFFECT_NONE || de == DROPEFFECT_COPY || de == DROPEFFECT_MOVE);
			PostMessage(WM_LBUTTONUP,nFlags,MAKELONG(point.x,point.y));
		}
	}
}


