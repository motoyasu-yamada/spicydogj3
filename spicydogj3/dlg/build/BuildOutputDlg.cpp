#include "stdafx.h"
#include "resource.h"
#include "BuildOutputDlg.h"
#include "model/build/BuildOutput.h"
#include "SpicydogDoc.h"
#include "utility/StringUtility.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF IMAGELISTMASK = RGB(0x00,0x80,0x80);
const int IMAGELIST_CX = 16;
const int COLWIDTH_STATE = 64;
const int COLWIDTH_LOC   = 128;
const int COLWIDTH_MSG   = 256;

IMPLEMENT_DYNCREATE(BuildOutputDlg,DialogBarDlg)
BEGIN_MESSAGE_MAP(BuildOutputDlg,DialogBarDlg)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_COLUMNCLICK, IDCL_OUTPUT, OnColumnclickOutput)
END_MESSAGE_MAP()

BuildOutputDlg::BuildOutputDlg()
:	m_state(BuildOutput::STATE_NONE),
	m_pBuildOutput(NULL),
	m_bInited(FALSE) {
}

BuildOutputDlg::~BuildOutputDlg() {
}

void BuildOutputDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCL_OUTPUT, m_listOutput);
}

BOOL BuildOutputDlg::OnInitDialog()  {
	DialogBarDlg::OnInitDialog();
	if (!m_images.Create(IDB_BUILD,IMAGELIST_CX,1,IMAGELISTMASK)) {
		return FALSE;
	}
	SetupList();
	OnNotify(SpicydogDoc::EVENTNOTIFYCODE_BUILT,NULL,0,NULL);
	m_bInited = TRUE;
	return TRUE;
}

void InsertColumn(CListCtrl& list,int col,UINT name,int fmt,int width) {
	CString str = FormatString(name);
	if (list.InsertColumn(col,str,fmt,width,col) == -1) {
		ASSERT(FALSE);
		AfxThrowResourceException();
	}
}

void BuildOutputDlg::SetupList() {
	m_listOutput.SetExtendedStyle(m_listOutput.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_listOutput.SetImageList(&m_images,LVSIL_SMALL);

	InsertColumn(m_listOutput,0,IDS_BUILDOUTPUTDLG_LIST_STATE,LVCFMT_LEFT,COLWIDTH_STATE);
	InsertColumn(m_listOutput,1,IDS_BUILDOUTPUTDLG_LIST_LOC,  LVCFMT_LEFT,COLWIDTH_LOC);
	InsertColumn(m_listOutput,2,IDS_BUILDOUTPUTDLG_LIST_MSG,  LVCFMT_LEFT,COLWIDTH_MSG);
}

void BuildOutputDlg::OnDocumentActivated(CDocument* p) {
	DialogBarDlg::OnDocumentActivated(p);
	m_listOutput.EnableWindow(ExistActiveDocument());
	OnNotify(SpicydogDoc::EVENTNOTIFYCODE_BUILT,p,0,0);
}

void BuildOutputDlg::OnNotify(DWORD dwNotifyCode,CDocument* p,LPARAM,LPVOID) {
	if (dwNotifyCode != SpicydogDoc::EVENTNOTIFYCODE_BUILT) {
		return;
	}
	if (!ExistActiveDocument()) {
		m_listOutput.DeleteAllItems();	
		m_state = BuildOutput::STATE_NONE;
		return;
	}
	if (p == NULL) {
		ASSERT(FALSE);
		return;
	}
	SpicydogDoc* document = DYNAMIC_DOWNCAST(SpicydogDoc,p);
	ASSERT_VALID(document);
	if (document == NULL) {
		ASSERT(FALSE);
		return;
	}

	m_pBuildOutput = &document->buildOutput;
	m_state = m_pBuildOutput->GetResult();

	m_listOutput.SetRedraw(FALSE);
	
		int nSel = GetListCtrlCurSel(m_listOutput);

		m_listOutput.DeleteAllItems();
		const int nc = m_pBuildOutput->GetItemCount();
		for (int n = 0;n < nc;n++) {
			const BUILD_OUTPUT_ITEM  i = m_pBuildOutput->GetItem(n);
			const CString text         = BuildOutput::BuildOutputItem2Text(i.m_nType);
			const int image            = BuildOutput::BuildOutputItem2Icon(i.m_nType);
			m_listOutput.InsertItem (n,text,image);
			m_listOutput.SetItem    (n,0,LVIF_TEXT | LVIF_IMAGE | LVIF_STATE,text,image,
				                     (nSel == n) ? LVIS_SELECTED : 0,LVIS_SELECTED,0);
			m_listOutput.SetItemText(n,1,i.m_strLoc);
			m_listOutput.SetItemText(n,2,i.m_strMsg);
			m_listOutput.SetItemData(n,n);
		}

		if (nSel != -1) {
			m_listOutput.EnsureVisible(nSel,FALSE);
		}

	m_listOutput.SetRedraw(TRUE);
}

int CALLBACK BuildOutputDlg::SortListCallback(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort) {
	BuildOutputDlg* pThis = (BuildOutputDlg*)lParamSort;
	if (pThis == NULL)
		{ASSERT(FALSE);return -1;}
	return pThis->OnSortList(lParam1,lParam2);
}

int Compare(int n1,int n2) {
	if (n1 < n2)
		return -1;
	if (n2 < n2)
		return 1;
	return 0;
}

int BuildOutputDlg::OnSortList(int nL,int nR) {
	if (m_pBuildOutput == NULL)
		{ASSERT(FALSE);return -1;}

	const BUILD_OUTPUT_ITEM& l = m_pBuildOutput->GetItem(nL);
	const BUILD_OUTPUT_ITEM& r = m_pBuildOutput->GetItem(nR);

	switch(m_SortType) {
	case SORT_BY_TYPE:
		return Compare(l.m_nType,r.m_nType);
	case SORT_BY_LOC:
		return l.m_strLoc.Compare(r.m_strLoc);
	case SORT_BY_MSG:
		return l.m_strMsg.Compare(r.m_strMsg);
	default:
		ASSERT(FALSE);
		return -1;
	}
}

void BuildOutputDlg::SortList(SORTTYPE st) {
	m_SortType = st;
	VERIFY(m_listOutput.SortItems(SortListCallback,(LPARAM)this));
}

void BuildOutputDlg::OnSize(UINT nType, int cx, int cy) {
	CDialog::OnSize(nType, cx, cy);
	if (!m_bInited) {
		return;
	}
	StretchChildWindowToRight(m_listOutput,cx);
	StretchChildWindowToBottom(m_listOutput,cy);
}

void BuildOutputDlg::OnColumnclickOutput(NMHDR* pNMHDR, LRESULT* pResult) {
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	ASSERT(pNMListView->iItem == -1);
	int col =pNMListView->iSubItem;	
	*pResult = 0;
	m_SortType = (SORTTYPE)col;
	VERIFY(m_listOutput.SortItems(SortListCallback,(DWORD)this));
}
