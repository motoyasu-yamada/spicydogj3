#include "stdafx.h"
#include "DialogSizingBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(DialogSizingBar, SECControlBar)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(DialogSizingBar,SECControlBar)
DialogSizingBar::DialogSizingBar()
:	m_pDialog(NULL),
	m_pMgr(NULL)
{
}

DialogSizingBar::~DialogSizingBar() {
	ASSERT(m_pDialog == NULL);
	ASSERT(m_pMgr == NULL);
}

void DialogSizingBar::PostNcDestroy() {
	if (m_pMgr != NULL) {
		m_pMgr->Remove(this);
		m_pMgr = NULL;
	}
	SECControlBar::PostNcDestroy();
	delete this;
}

void DialogSizingBar::AttachDialog(CDialog* pDialog) {
	ASSERT(m_pDialog == NULL);
	ASSERT_VALID(pDialog);
	m_pDialog = pDialog;
	ASSERT(m_pDialog->GetParent() == this);
	ASSERT(m_pDialog->GetOwner() == this);
	AdjustDialog();
}

void DialogSizingBar::AdjustDialog() {
	if (m_pDialog == NULL)
		return;
	ASSERT_VALID(m_pDialog);
	CRect rc;
	GetInsideRect(rc);
	m_pDialog->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	m_pDialog->Invalidate();
}

void DialogSizingBar::OnSize(UINT nType, int cx, int cy) {
	SECControlBar::OnSize(nType, cx, cy);
	AdjustDialog();
}

void DialogSizingBar::OnWindowPosChanged(WINDOWPOS* p) {
	SECControlBar::OnWindowPosChanged(p);
	AdjustDialog();
}

void DialogSizingBar::OnDestroy()  {
	VERIFY(m_pDialog->DestroyWindow());
	delete m_pDialog;
	m_pDialog = NULL;
	SECControlBar::OnDestroy();	
}

BOOL DialogSizingBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	if (m_hWnd == NULL)
		return FALSE;

	if (IsWindowVisible() && m_pDialog != NULL && m_pDialog->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return TRUE;
	if (SECControlBar::OnCmdMsg(nID,nCode,pExtra,pHandlerInfo))
		return TRUE;
	return FALSE;
}

void DialogSizingBar::OnUpdateCmdUI(CFrameWnd* pFrameWnd,BOOL b) {
	if (m_pDialog == NULL) {
		SECControlBar::OnUpdateCmdUI(pFrameWnd,b);
		return;
	}

	ASSERT_VALID(m_pDialog);
	BOOL bEnabled = m_pDialog->IsWindowEnabled();
	CCmdUI state;
	state.m_nID = m_pDialog->GetDlgCtrlID();
	state.m_pOther = m_pDialog;
	if (!state.DoUpdate(pFrameWnd,FALSE)) {
		state.DoUpdate(m_pDialog,FALSE);
	}
	m_pDialog->UpdateDialogControls(pFrameWnd,TRUE);
}

DialogSizingBarMgr::DialogSizingBarMgr() : m_pActiveBar(NULL) {
}

void DialogSizingBarMgr::Append(DialogSizingBar* p) {
	ASSERT(p != NULL);
	m_listBars.push_back(BARPTR(p));
	ASSERT(p->m_pMgr == NULL);
	p->m_pMgr = this;
}

void DialogSizingBarMgr::Remove(DialogSizingBar* p) {
	ASSERT_VALID(p);
	ASSERT(p->m_pMgr == this);
	BARS::iterator i = std::find(m_listBars.begin(),m_listBars.end(),p);
	if (i != m_listBars.end()) {
		ASSERT((*i)->m_pMgr == this);
		m_listBars.erase(i);
	} else {
		ASSERT(FALSE);
	}
}

BOOL DialogSizingBarMgr::OnCmdMsg1(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHI) {
	m_pActiveBar = FindActiveBar();
	if (m_pActiveBar != NULL) {
		return m_pActiveBar->OnCmdMsg(nID,nCode,pExtra,pHI);
	} else {
		return FALSE;
	}
}

DialogSizingBar* DialogSizingBarMgr::FindActiveBar() {
	CWnd* pFocus = CWnd::GetFocus();
	BARS::const_iterator i;
	for(i =  m_listBars.begin();i != m_listBars.end();i++) {
		DialogSizingBar* pBar = *i;
		if (pBar->IsChild(pFocus) || pFocus == pBar) {
			return pBar;
		}
	}
	return NULL;
}

BOOL DialogSizingBarMgr::OnCmdMsg2(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHI) {
	BARS::const_iterator i;
	for(i =  m_listBars.begin();i != m_listBars.end();i++) {
		DialogSizingBar* pBar = *i;
		if (pBar != m_pActiveBar && pBar->OnCmdMsg(nID,nCode,pExtra,pHI)) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL DialogSizingBarMgr::IsAnyBarActive() const {
	return m_pActiveBar != NULL;
}

HACCEL DialogSizingBarMgr::GetDefaultAccelerator() {
	if (m_pActiveBar == NULL)
		return NULL;
	return m_pActiveBar->GetDefaultAccelerator();
}

BOOL DialogSizingBarMgr::OnCommand1(WPARAM wParam, LPARAM lParam) {
	m_pActiveBar = FindActiveBar();
	if (m_pActiveBar != NULL)
		return m_pActiveBar->OnCommand(wParam,lParam);
	else
		return FALSE;
}

BOOL DialogSizingBarMgr::OnCommand2(WPARAM wParam, LPARAM lParam) {
	return FALSE;
	BARS::const_iterator i;
	for(i =  m_listBars.begin();i != m_listBars.end();i++) {
		DialogSizingBar* pBar = *i;
		if (pBar != m_pActiveBar && pBar->OnCommand(wParam,lParam)) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL DialogSizingBar::PreTranslateMessage(MSG* pMsg) {
	if (SECControlBar::PreTranslateMessage(pMsg))
		return TRUE;

	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) {
		HACCEL hAccel = GetDefaultAccelerator();
		return hAccel != NULL &&
		   ::TranslateAccelerator(AfxGetMainWnd()->m_hWnd, hAccel, pMsg);
	}
	return FALSE;

}

