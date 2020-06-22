#include "stdafx.h"
#include "resource.h"

#include "HistoryDlg.h"
#include "cmd/Cmd.h"
#include "SpicydogDoc.h"
#include "utility/MemDC.h"
#include "utility/WndUtility.h"
#include "cmd/CmdHistory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF COLOR_MASK = RGB(0x00,0x80,0x80);
const int HISTORY_LIST_ITEM_HEIGHT = 20;
const int COLWIDTH_INDICATOR = 20;
const int COLWIDTH_ICON      = 20;
const int BMPSIZE_HISTORY = 20;

BEGIN_MESSAGE_MAP(HistoryDlg, DialogBarDlg)
	ON_NOTIFY(LVN_GETDISPINFO, IDCL_HISTORY, OnOwnerData)
	ON_NOTIFY(NM_DBLCLK, IDCL_HISTORY, OnDblclkHistory)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_SIZE()
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(HistoryDlg,DialogBarDlg)
HistoryDlg::HistoryDlg() {
	m_activeDocument = NULL;
	m_curCmd  = -1;
	m_history = NULL; 
	m_inited  = FALSE;
}

HistoryDlg::~HistoryDlg() {
}

void HistoryDlg::DoDataExchange(CDataExchange* pDX) {
	DialogBarDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDCL_HISTORY,m_list);
}

void HistoryDlg::OnSize(UINT nType, int cx, int cy) {
	DialogBarDlg::OnSize(nType, cx, cy);
	if (!m_inited) {
		return;
	}
	AdjustSize();
}

BOOL HistoryDlg::OnInitDialog() {
	DialogBarDlg::OnInitDialog();

	if (!m_imagesCmd.Create(IDB_COMMAND,16,16,COLOR_MASK)) {
		ASSERT(FALSE);
		return FALSE;
	}
	if (!m_imagesBg.Create (IDB_HISTORY,BMPSIZE_HISTORY,BMPSIZE_HISTORY,COLOR_MASK)) {
		ASSERT(FALSE);
		return FALSE;
	}
	if (!m_tools.Create(IDT_HISTORY,this,-1,WS_CHILD|WS_VISIBLE,1)) {
		ASSERT(FALSE);
		return FALSE;
	}
	
	VERIFY(m_penBorder.CreatePen(PS_SOLID,0,RGB(0x00,0x00,0x00)));
	m_list.InsertColumn(0,NULL,LVCFMT_LEFT);

	AdjustSize();
	m_inited = TRUE;
	return TRUE;
}

void HistoryDlg::AdjustSize() {
	CRect rc;
	GetClientRect(rc);

	MoveChildWindowToBottom(m_tools,rc.bottom);
	StretchChildWindowToRight(m_list,rc.right);
	StretchChildWindowToBottom(m_list,rc.bottom-GetWindowHeight(m_tools));
	AdjustListColumnSize();
	Invalidate();
}

void HistoryDlg::AdjustListColumnSize() {
	CRect rcC;
	m_list.GetClientRect(rcC);
	m_list.SetColumnWidth(0,rcC.Width());
}

void HistoryDlg::OnDocumentActivated(CDocument* pDoc) {
	DialogBarDlg::OnDocumentActivated(pDoc);
	m_activeDocument = pDoc;
	if (m_activeDocument == NULL) {
		m_history = NULL;
		m_curCmd  = 0;
		m_list.SetItemCountEx(0,LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
		AdjustListColumnSize();
		EnableDescendantWindows(this,FALSE);
		return;
	}
	m_history = dynamic_cast<CmdHistory*>(m_activeDocument);
	EnableDescendantWindows(this,TRUE);
}

int GetListCtrlSingleCurSel(CListCtrl& ctrl) {
	ASSERT(ctrl.GetSelectedCount() <= 1);
	POSITION pos = ctrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return -1;
	return ctrl.GetNextSelectedItem(pos);
}

void HistoryDlg::OnDocumentUpdated(CDocument* pDoc,LPARAM lHint,CObject*) {
	ASSERT(IsWindow(m_hWnd));
	if(pDoc == NULL || m_activeDocument != pDoc)
		return;
	if (lHint != 0)
		return;
	
	m_list.SetItemCountEx(m_history->GetCmdCount(),LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL );
	AdjustListColumnSize();
	if (m_history->GetCmdCount() == 0) {
		m_curCmd = GetListCtrlSingleCurSel(m_list);
	} else {
		m_curCmd = m_history->GetCurCmdPos();
	}
	m_list.Invalidate();
}

void HistoryDlg::OnOwnerData(NMHDR*,LRESULT* pResult) {
	*pResult = 0;
}

void HistoryDlg::OnDblclkHistory(NMHDR* _p, LRESULT* pResult) {
	ASSERT(m_history != NULL);
	NMITEMACTIVATE* p = (NMITEMACTIVATE*)_p;
	ASSERT(AfxIsValidAddress(p,sizeof NMITEMACTIVATE));
	int iItem = p->iItem;
	ASSERT(-1 <= iItem && iItem < m_history->GetCmdCount());
	m_history->Jump2Cmd(iItem);
	((SpicydogDoc*)DYNAMIC_DOWNCAST(SpicydogDoc,m_activeDocument))->UpdateAll(NULL,0);
	*pResult = 0;
}

void HistoryDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT pD) {
	if (nIDCtl != IDCL_HISTORY) {
		DialogBarDlg::OnDrawItem(nIDCtl,pD);
		return;
	}
	ASSERT(m_activeDocument != NULL);
	
	int nItem = pD->itemID;
	int nItemCount = m_history->GetCmdCount();
	ASSERT(0 <= nItem && nItem < nItemCount);
	CRect rcItem;
	m_list.GetItemRect(nItem,rcItem,LVIR_BOUNDS);
	CRect rcWnd;
	m_list.GetClientRect(rcWnd);
	rcItem.right = min(rcItem.right,rcWnd.right);
	
	MemDC dc(CDC::FromHandle(pD->hDC),&rcItem); {
		const Cmd* cmd  = m_history->GetCmd(nItem);
		CString strItem = cmd->GetName();
		int nItemImage  = cmd->GetIconIndex();
		int nDCState    = dc.SaveDC();

		COLORREF crBg;
		COLORREF crText;
		if (pD->itemState & ODS_SELECTED) {
			crBg	= ::GetSysColor(COLOR_HIGHLIGHT);
			crText	= ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		} else if (m_curCmd < nItem) {
			crBg	= ::GetSysColor(COLOR_APPWORKSPACE);
			crText	= ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		} else {
			crBg	= ::GetSysColor(COLOR_WINDOW);
			crText	= ::GetSysColor(COLOR_WINDOWTEXT);
		}

		dc.FillSolidRect(rcItem,crBg);
	
		m_imagesCmd.Draw(&dc,nItemImage,CPoint(rcItem.left + 16,rcItem.top),ILD_TRANSPARENT);
	
		CRect rcText(rcItem);
		rcText.left += 36;	
		dc.SelectObject(GetFont());
		dc.SetTextColor(crText);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(strItem,rcText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		m_imagesBg.Draw(&dc,nItem == m_curCmd ? 1:0,CPoint(rcItem.left,rcItem.top),ILD_TRANSPARENT);
	
		if (m_curCmd == nItem || nItem == (nItemCount-1)) {
			dc.SelectObject(&m_penBorder);
			dc.MoveTo(rcItem.left,rcItem.bottom-1);
			dc.LineTo(rcItem.right,rcItem.bottom-1);
		}
		VERIFY(dc.RestoreDC(nDCState));
	}
}

void HistoryDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT pM) {
	if (nIDCtl != IDCL_HISTORY) {
		DialogBarDlg::OnMeasureItem(nIDCtl,pM);
		return;
	}
	pM->itemHeight = HISTORY_LIST_ITEM_HEIGHT;
}
