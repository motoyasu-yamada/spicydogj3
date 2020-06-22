#include "stdafx.h"
#include "ToolBarWnd.h"
#include "AfxPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct CToolBarData
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;
	WORD* items() {return (WORD*)(this+1); }
};

BEGIN_MESSAGE_MAP(ToolBarWnd, CWnd)
	ON_NOTIFY(TTN_NEEDTEXT,0,OnNeedText)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(ToolBarWnd,CWnd)
ToolBarWnd::ToolBarWnd()
{
}

void ToolBarWnd::OnNeedText(NMHDR* pN,LRESULT* pR)
{
	LPTOOLTIPTEXT pT = (LPTOOLTIPTEXT) pN;
	UINT nStringID = pT->hdr.idFrom;

	TCHAR szFullText[256];
	CString strTipText;
	AfxLoadString(nStringID,szFullText);
	AfxExtractSubString(strTipText,szFullText,1,'\n');
	strcpy(pT->lpszText,strTipText);
	*pR = TRUE;
}

const DWORD TBWSTYLE_DEFAULT = WS_VISIBLE | 
							   WS_CHILD | 
							   WS_CLIPSIBLINGS |
							   CCS_NODIVIDER  |
							   //CCS_NOPARENTALIGN | 
							   CCS_NOMOVEY |
							   //CCS_NORESIZE |
							   TBSTYLE_TOOLTIPS |
							   0; //CCS_TOP;

void ToolBarWnd::Create(DWORD dwStyle,CPoint pt,CWnd* pParent,UINT nID)
{
	if (!CWnd::Create(NULL,NULL,WS_VISIBLE | WS_CHILD,CRect(pt,CSize(0,0)),pParent,nID))
		::AfxThrowResourceException();
	UpdateWindow();

	dwStyle |= TBWSTYLE_DEFAULT;
	if (!m_ctrl.Create(dwStyle,CRect(0,0,0,0),this,nID))
		::AfxThrowResourceException();
	m_ctrl.UpdateWindow();
	m_ctrl.ShowWindow(SW_SHOW);
}

CToolBarData* LoadToolBarData(UINT nResID)
{
	ASSERT(nResID != 0);

	HRSRC hrsrc = ::FindResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(nResID),RT_TOOLBAR);
	if (hrsrc == NULL)
		::AfxThrowResourceException();

	HGLOBAL hg = LoadResource(AfxGetInstanceHandle(),hrsrc);
	if (hg == NULL)
		::AfxThrowResourceException();

	CToolBarData* pData = (CToolBarData*)LockResource(hg);
	ASSERT(AfxIsValidAddress(pData,sizeof CToolBarData,FALSE));
	ASSERT(pData->wVersion == 1);

	return pData;
}

void ToolBarWnd::LoadToolBar(UINT nResID,int nCol,int cx,int cy)
{
	ASSERT_VALID(this);
	ASSERT(nResID != 0);
	ASSERT(0 <= nCol);
	m_nCols = nCol;

	CToolBarData* pData = LoadToolBarData(nResID);
	ASSERT(AfxIsValidAddress(pData,sizeof CToolBarData,FALSE));
	ASSERT(pData->wVersion == 1);

	if (m_ctrl.AddBitmap(pData->wItemCount,nResID) == -1)
		::AfxThrowResourceException();

	CArray<UINT,UINT&> aItems;
	LoadItems(pData->items(),pData->wItemCount,aItems);

	DeleteAllButtons();
	SetButtons(aItems.GetData(),pData->wItemCount);

	m_ctrl.SetBitmapSize(CSize(pData->wWidth,pData->wHeight));
	m_ctrl.SetButtonSize(CSize(pData->wWidth + cx,pData->wHeight + cy));
	m_ctrl.SetAnchorHighlight(FALSE);
	m_ctrl.SetIndent(0);

	AdjustSize(pData->wWidth,pData->wHeight,cx,cy);
}

void ToolBarWnd::AdjustSize(int bx,int by,int mx,int my)
{
	m_ctrl.AutoSize();

	CPoint ptMargin(0,0);

	CSize szWnd = ptMargin;
	int cx = bx + mx * 2;
	int cy = by + my * 2;
	if (m_nCols == 0)
	{
		szWnd.cx += cx * m_ctrl.GetButtonCount();
		szWnd.cy += cy;
	}
	else
	{
		szWnd.cx += cx * m_nCols;
		szWnd.cy += cy * (m_ctrl.GetButtonCount()+ m_nCols -1) / m_nCols;
	}

	SetWindowPos(NULL,0,0,szWnd.cx,szWnd.cy,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	m_ctrl.AutoSize();
	CRect rc;
	m_ctrl.GetClientRect(rc);
	SetWindowPos(NULL,0,0,rc.Width(),rc.Height(),SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void ToolBarWnd::LoadItems(WORD *pItems,int nCount,CArray<UINT,UINT&>& aItems)
{	
	ASSERT(AfxIsValidAddress(pItems,sizeof WORD * nCount,FALSE));
	aItems.SetSize(nCount);
	for (int n = 0;n < nCount;n++)
	{
		aItems[n] = pItems[n];
	}
}

void ToolBarWnd::DeleteAllButtons()
{
	ASSERT_VALID(this);
	int n = m_ctrl.GetButtonCount();
	while (n --)
		m_ctrl.DeleteButton(0);
}

void ToolBarWnd::SetButtons(const UINT* pItems,int nCount)
{
	ASSERT(AfxIsValidAddress(pItems,sizeof UINT * nCount,FALSE));

	TBBUTTON b;
	int nImage = 0;
	for (int n = 0;n < nCount;n++)
	{
		ZeroMemory(&b,sizeof b);
		b.fsState = TBSTATE_ENABLED;
		if (m_nCols != 0 && ((n+1) % m_nCols) == 0)
			b.fsState |= TBSTATE_WRAP;

		if ((b.idCommand = pItems[n]) == 0)
		{
			b.fsStyle = TBSTYLE_SEP;
			b.iBitmap = 8;
		}
		else
		{
			b.fsStyle = TBSTYLE_BUTTON;
			b.iBitmap = nImage++;
		}
		if (!m_ctrl.AddButtons(1,&b))
			::AfxThrowResourceException();
	}
}

void ToolBarWnd::AssertValid() const {
	CWnd::AssertValid();
	ASSERT_VALID(&m_ctrl);
}

CToolBarCtrl& ToolBarWnd::GetToolBarCtrl() const  {
	ASSERT_VALID(this);
	return const_cast<CToolBarCtrl&>(m_ctrl);
}

class ToolBarWndCmdUI : public CCmdUI {
public:
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText){}
	virtual void SetRadio(BOOL b);
};

void ToolBarWndCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	CToolBarCtrl& ctrl = *(CToolBarCtrl*)DYNAMIC_DOWNCAST(CToolBarCtrl,m_pOther);
	ASSERT_VALID(&ctrl);
	ASSERT(m_nIndex < m_nIndexMax);
	BOOL bOld = ctrl.IsButtonEnabled(m_nID);
	if (!bOld && bOn || bOld && !bOn)
		ctrl.EnableButton(m_nID,bOn);
}

void ToolBarWndCmdUI::SetCheck(int nCheck)
{
	m_bEnableChanged = TRUE;
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
	CToolBarCtrl& ctrl = *(CToolBarCtrl*)DYNAMIC_DOWNCAST(CToolBarCtrl,m_pOther);
	ASSERT_VALID(&ctrl);
	ASSERT(m_nIndex < m_nIndexMax);
	switch (nCheck)
	{
	case 0:
		ctrl.CheckButton(m_nID,FALSE);
		break;
	case 1:
		ctrl.CheckButton(m_nID,TRUE);
		break;
	case 2:
		ctrl.Indeterminate(m_nID,TRUE);
		break;
	}
}

void ToolBarWndCmdUI::SetRadio(BOOL b)
{
	SetCheck(b ? 1 : 0);
}

void ToolBarWnd::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) {
	ToolBarWndCmdUI state;
	state.m_pOther = &m_ctrl;

	state.m_nIndexMax = m_ctrl.GetButtonCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
	{
		TBBUTTON button;
		m_ctrl.GetButton(state.m_nIndex, &button);
		state.m_nID = button.idCommand;

		if (!(button.fsStyle & TBSTYLE_SEP))
		{
			if (m_ctrl.OnCmdMsg(0,
				MAKELONG((int)CN_UPDATE_COMMAND_UI, WM_COMMAND+WM_REFLECT_BASE),
				&state, NULL))
				continue;

			if (m_ctrl.OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
				continue;
			state.DoUpdate(pTarget, bDisableIfNoHndler);
		}
	}
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

LRESULT ToolBarWnd::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM) {
	if ((GetStyle() & WS_VISIBLE)) {
		CFrameWnd* pTarget = (CFrameWnd*)GetOwner();
		if (pTarget == NULL || !pTarget->IsFrameWnd())
			pTarget = GetTopLevelFrame();
		if (pTarget != NULL)
			OnUpdateCmdUI(pTarget,wParam);
	}
	return 0L;
}

void ToolBarWnd::SetOwner(CWnd* pOwnerWnd) {
	ASSERT_VALID(this);
	if (m_hWnd != NULL) {
		ASSERT(::IsWindow(m_hWnd));
		m_ctrl.SetOwner(pOwnerWnd);
	}
	CWnd::SetOwner(pOwnerWnd);
}

BOOL ToolBarWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) {
		return TRUE;
	}
	if (GetOwner() != NULL) {
		return GetOwner()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	return FALSE;
}
