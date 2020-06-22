
#include "stdafx.h"
#include "CntMgr.h"
#include "Cnt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CntMgr::CntMgr()
:	m_nActiveID(0),
	m_pActive(NULL),
	m_nDefaultID(0) {
}

CntMgr::~CntMgr() {
	m_Controlers.clear();
}

void CntMgr::SetDefaultControlerID(UINT nID) {
	ASSERT(nID != 0);
	m_nDefaultID = nID;
	ASSERT(IsValidControlerID(m_nDefaultID));
}

void CntMgr::ActivateDefaultControler() {
	ASSERT(m_nDefaultID != 0);
	ASSERT(IsValidControlerID(m_nDefaultID));
	ActivateControler(m_nDefaultID);
}

BOOL CntMgr::IsValidControlerID(UINT nID) const {
	return nID != 0 && m_Controlers.find(nID) != m_Controlers.end();
}

void CntMgr::AppendControler(UINT nID,Cnt* p) {
	ASSERT(!IsValidControlerID(nID));
	ASSERT(AfxIsValidAddress(p,sizeof Cnt));

	CONTROLERS::iterator i = m_Controlers.find(nID);
	ASSERT(i == m_Controlers.end());
	m_Controlers[nID] = CONTROLERPTR(p);
	p->AttachControlerMgr(this);
}

void CntMgr::RemoveControler(UINT nID) {
	ASSERT(IsValidControlerID(nID));

	CONTROLERS::iterator i = m_Controlers.find(nID);
	ASSERT(i != m_Controlers.end());
	m_Controlers.erase(nID);
}

Cnt* CntMgr::GetActiveControler() const {
	ASSERT(m_pActive != NULL);
	ASSERT(AfxIsValidAddress(m_pActive,sizeof Cnt));
	return m_pActive;
}

UINT CntMgr::GetActiveControlerID() const {
	ASSERT(IsValidControlerID(m_nActiveID));
	ASSERT(AfxIsValidAddress(m_pActive,sizeof Cnt));
	return m_nActiveID;
}

void CntMgr::ActivateControler(UINT nID) {
	ASSERT(IsValidControlerID(nID));
	if (nID == m_nActiveID)
		return;

	CONTROLERS::iterator i = m_Controlers.find(nID);
	ASSERT(i != m_Controlers.end());

	if (m_pActive != NULL) {
		m_pActive->OnExit();
		m_pActive = NULL;
	}

	m_nActiveID = nID;
	m_pActive = (i->second).get();
	ASSERT(IsValidControlerID(m_nActiveID));
	ASSERT(AfxIsValidAddress(m_pActive,sizeof Cnt));

	m_pActive->OnEnter();
}

BOOL CntMgr::DoCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHI) {
	if (IsValidControlerID(nID)) {
		if (nCode == CN_UPDATE_COMMAND_UI) {
			return DoUpdateCmdUI(nID,pExtra,pHI);
		}
		if (nCode == CN_COMMAND) {
			return DoCmd(nID,pExtra,pHI);
		}
	} else {
		Cnt* pBase = GetActiveControler();
		if (pBase != NULL) {
			return pBase->OnCmdMsg(nID,nCode,pExtra,pHI);
		}
	}
	return FALSE;
}

BOOL CntMgr::DoCmd(UINT nID,void* pExtra, AFX_CMDHANDLERINFO* pHI) {
	ASSERT(IsValidControlerID(nID));
	if (nID == m_nActiveID)
		return FALSE;

	ActivateControler(nID);
	return TRUE;
}

BOOL CntMgr::DoUpdateCmdUI(UINT nID,void* pExtra, AFX_CMDHANDLERINFO* pHI) {
	ASSERT(m_Controlers.find(nID) != m_Controlers.end());
	ASSERT(pExtra != NULL);
	if (GetActiveControler()->OnCmdMsg(nID,CN_UPDATE_COMMAND_UI,pExtra,pHI))
		return TRUE;

	CCmdUI* pCmdUI = (CCmdUI*)pExtra;
	ASSERT(AfxIsValidAddress(pExtra,sizeof CCmdUI));
	ASSERT(!pCmdUI->m_bContinueRouting);
	Cnt* pCont = m_Controlers.find(nID)->second.get();
	ASSERT(AfxIsValidAddress(pCont,sizeof Cnt));
	if (!pCont->OnCmdMsg(nID,CN_UPDATE_COMMAND_UI,pExtra,pHI)) {
		pCmdUI->Enable(TRUE);
	}
	pCmdUI->SetRadio(nID == m_nActiveID);
	return TRUE;
}

HACCEL CntMgr::GetDefaultAccelerator() {
	//return GetActiveControler()->GetDefaultAccelerator();
	return NULL;
}

BOOL CntMgr::DoSetCursor(CWnd* pWnd,UINT nHitTest,UINT msg) {
	return GetActiveControler()->OnSetCursor(pWnd,nHitTest,msg);
}

void CntMgr::DoDraw(CDC* pDC) {
	GetActiveControler()->OnDraw(pDC);
}

BOOL CntMgr::DoPreTranslateMessage(MSG* pmsg) {
	ASSERT(AfxIsValidAddress(pmsg,sizeof MSG));

	Cnt* pControler = GetActiveControler();
	ASSERT(AfxIsValidAddress(pControler,sizeof Cnt));
	pControler->ContinueMsgRouting(FALSE);
	WPARAM& wParam = pmsg->wParam;
	LPARAM& lParam = pmsg->lParam;
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	
	switch(pmsg->message) {
	case WM_MOUSEMOVE:
		pControler->OnMouseMove(wParam,pt);
		break;
	case WM_LBUTTONDOWN:
		pControler->OnLButtonDown(wParam,pt);
		break;
	case WM_LBUTTONUP:
		pControler->OnLButtonUp(wParam,pt);
		break;
	case WM_LBUTTONDBLCLK:
		pControler->OnLButtonDblClk(wParam,pt);
		break;
	case WM_RBUTTONDOWN:
		pControler->OnRButtonDown(wParam,pt);
		break;
	case WM_RBUTTONUP:
		pControler->OnRButtonUp(wParam,pt);
		break;
	case WM_RBUTTONDBLCLK:
		pControler->OnRButtonDblClk(wParam,pt);
		break;
	case WM_KEYDOWN:
		pControler->OnKeyDown(wParam,LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_KEYUP:
		pControler->OnKeyUp(wParam,LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_SETCURSOR:
		pControler->OnSetCursor(CWnd::FromHandle((HWND)wParam),LOWORD(lParam),HIWORD(lParam));
		break;
	default:
		return FALSE;
	}
	return ! pControler->IsContinueMsgRouting();
}


class CTestCmdUI : public CCmdUI {
private:
	BOOL m_bEnabled;
public:
	CTestCmdUI() : m_bEnabled(TRUE) {}
public:
	BOOL IsEnabled() const { return m_bEnabled; }
	virtual void Enable(BOOL bOn) {
		m_bEnabled = bOn;
		m_bEnableChanged = TRUE;
	}
	virtual void SetCheck(int nCheck){}
	virtual void SetRadio(BOOL bOn){}
	virtual void SetText(LPCTSTR){}
}; // class CTestCmdUI

BOOL CntMgr::DoCommand(CWnd* pWnd,WPARAM wParam,LPARAM lParam) {
	ASSERT_VALID(pWnd);
	UINT nID      = LOWORD(wParam);
	HWND hWndCtrl = (HWND)lParam;
	int nCode     = HIWORD(wParam);

	if (hWndCtrl == NULL) {
		if (nID == 0) {
			return FALSE;
		}

		CTestCmdUI state;
		state.m_nID = nID;
		DoCmdMsg(nID, CN_UPDATE_COMMAND_UI, &state, NULL);
		if (!state.IsEnabled()) {
			ASSERT(FALSE);
			return TRUE;
		}

		nCode = CN_COMMAND;
	} else {
		ASSERT(nID == 0 || ::IsWindow(hWndCtrl));
		//if (_afxThreadState->m_hLockoutNotifyWindow == pWnd->m_hWnd) {
		//	return TRUE;
		//}
		//if (CWnd::ReflectLastMsg(hWndCtrl))
		//	return TRUE;
		if (nID == 0) {
			return FALSE;
		}
	}
	ASSERT(0 <= nCode || nCode == (int)0x8000);
	return DoCmdMsg(nID, nCode, NULL, NULL);
}
