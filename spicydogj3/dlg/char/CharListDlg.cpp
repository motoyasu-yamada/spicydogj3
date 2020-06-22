#include "stdafx.h"
#include "resource.h"
#include "CharListDlg.h"

#include "cmd/CmdSet.h"
#include "cmd/char/RenCharCmd.h"
#include "cmd/char/InsCharCmd.h"
#include "cmd/char/DelCharCmd.h"
#include "cmd/char/DupCharCmd.h"
#include "cmd/stage/AttachLayerCharCmd.h"
#include "cmd/stage/InsKeyFrameCmd.h"
#include "dlg/char/ImportPictCharDlg.h"
#include "dlg/char/NewEmptyPictCharDlg.h"
#include "model/char/CharList.h"
#include "model/Appli.h"
#include "model/AppliConf.h"
#include "model/char/TextChar.h"
#include "model/char/SndChar.h"
#include "model/char/BtnChar.h"
#include "model/char/PictChar.h"
#include "model/stage/Layer.h"
#include "model/stage/Stage.h"
#include "SpicydogDoc.h"
#include "utility/FileUtility.h"
#include "utility/StringUtility.h"
#include "utility/MemDC.h"
#include "utility/PaintMemDC.h"
#include "utility/WndUtility.h"
#include "utility/DDXUtility.h"
#include "utility/MemoryUtility.h"
#include "wnd/AppliFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class RenCharName : public CDialog {
	CString m_strName;
	
public:
	DECLARE_DYNAMIC(RenCharName)
	RenCharName(LPCTSTR,CWnd* pParent);
	CString GetNewName() const;

protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};

const int PREVIEW_LIST_HEIGHT = 64;
const int PREVIEW_LIST_MARGIN = 2;
const int PREVIEW_LIST_PREVIEW_HEIGHT = 60;
const int PREVIEW_LIST_PREVIEW_WIDTH  = 60;
const int PREVIEW_MARGIN_SIZE = 16;
const int CHARTYPE_ICON_SIZE = 16;
const COLORREF CHARTYPE_ICON_MASK = RGB(0x00,0x80,0x80);

BEGIN_MESSAGE_MAP(CharListDlg, DialogBarDlg)
	ON_COMMAND(ID_CHARLIST_VIEW_LIST,      OnListView)
	ON_COMMAND(ID_CHARLIST_VIEW_PREVIEW,   OnPreviewView)
	ON_COMMAND(ID_EDIT_CHAR,	OnEditChar)
	ON_COMMAND(ID_EDIT_CLEAR,OnEditClear)
	ON_COMMAND(ID_EDIT_COPY,OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT,OnEditCut)
	ON_COMMAND(ID_EDIT_DUP,		OnDupChar)
	ON_COMMAND(ID_EDIT_PASTE,OnEditPaste)
	ON_COMMAND(ID_INS_BTNCHAR,  OnInsBtnChar)
	ON_COMMAND(ID_INS_CHAR,OnInsCharMenu)
	ON_COMMAND(ID_INS_EMPTYPICTCHAR,OnInsEmptyPictChar)
	ON_COMMAND(ID_INS_PICTCHAR, OnInsPictChar)
	ON_COMMAND(ID_INS_SNDCHAR,  OnInsSndChar)
	ON_COMMAND(ID_INS_TEXTCHAR, OnInsTextChar)
	ON_COMMAND(ID_ATTACH_CHAR,	OnAttachChar)
	ON_COMMAND(ID_RENAME_CHAR,OnRenameChar)
	ON_NOTIFY(LVN_GETDISPINFO, IDCL_LIST, OnGetCharInfo)
	ON_NOTIFY(LVN_GETDISPINFO, IDCL_PREVIEW, OnGetCharInfo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDCL_LIST, OnSelChanged)
	ON_NOTIFY(LVN_ITEMCHANGED, IDCL_PREVIEW, OnSelChanged)
	ON_NOTIFY(NM_CLICK,        IDCL_LIST, OnClickCharList)
	ON_NOTIFY(NM_CLICK,        IDCL_PREVIEW, OnClickCharList)
	ON_NOTIFY(NM_DBLCLK,       IDCL_LIST, OnDblclkCharList)
	ON_NOTIFY(NM_DBLCLK,       IDCL_PREVIEW, OnDblclkCharList)
	ON_NOTIFY(NM_RCLICK,       IDCL_LIST, OnRclickCharList)
	ON_NOTIFY(NM_RCLICK,       IDCL_PREVIEW, OnRclickCharList)
	ON_NOTIFY(NM_RETURN,       IDCL_LIST, OnReturnCharList)
	ON_NOTIFY(NM_RETURN,       IDCL_PREVIEW, OnReturnCharList)
	ON_UPDATE_COMMAND_UI(ID_ATTACH_CHAR,OnUpdateAttachChar)
	ON_UPDATE_COMMAND_UI(ID_CHARLIST_VIEW_LIST,   OnUpdateView)
	ON_UPDATE_COMMAND_UI(ID_CHARLIST_VIEW_PREVIEW,OnUpdateView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CHAR,	OnUpdateRequireSelected)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR,	OnUpdateRequireSelected)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,	OnUpdateRequireSelected)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT,	OnUpdateRequireSelected)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DUP,	OnUpdateRequireSelected)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE,	OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_INS_BTNCHAR,OnUpdateExistActiveDocument)
	ON_UPDATE_COMMAND_UI(ID_INS_CHAR,OnUpdateExistActiveDocument)
	ON_UPDATE_COMMAND_UI(ID_INS_EMPTYPICTCHAR,OnUpdateExistActiveDocument)
	ON_UPDATE_COMMAND_UI(ID_INS_PICTCHAR,OnUpdateExistActiveDocument)
	ON_UPDATE_COMMAND_UI(ID_INS_SHAPECHAR,OnUpdateExistActiveDocument)
	ON_UPDATE_COMMAND_UI(ID_INS_SNDCHAR,OnUpdateExistActiveDocument)
	ON_UPDATE_COMMAND_UI(ID_INS_TEXTCHAR,OnUpdateExistActiveDocument)
	ON_UPDATE_COMMAND_UI(ID_RENAME_CHAR,OnUpdateRequireSingleSelected)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_SAFX_SPLITV_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CharListDlg,DialogBarDlg)
CharListDlg::CharListDlg()
:	DialogBarDlg(),
	m_bInit(FALSE),
	m_document(NULL),
	m_viewMode(VIEW_MODE_PREVIEW) {
}

CharListDlg::~CharListDlg() {
}


void CharListDlg::DoDataExchange(CDataExchange* pDX) {
	DialogBarDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDCL_PREVIEW, m_listPreview);
	DDX_Control(pDX, IDCL_LIST,    m_listList);
	DDX_Control(pDX, IDCS_CHAR_PREVIEW, m_stPreview);
	DDX_Control(pDX, IDCS_SPLITTER, m_splitter);
}

BOOL CharListDlg::OnInitDialog() {
	DialogBarDlg::OnInitDialog();
	SetupResources();
	SetupListListCtrl();
	SetupPreviewListCtrl();
	SetupPreviewStatic();
	VERIFY(m_itb.Create(IDT_CHARLIST,this,-1,WS_CHILD | WS_VISIBLE,1));

	m_bInit = TRUE;
	SwitchViewMode(VIEW_MODE_LIST,TRUE);
	return TRUE;
}

void CharListDlg::SetupResources() {
	LOGFONT lf;
	ZeroMemory(&lf,sizeof LOGFONT);
	if (SystemParametersInfo(SPI_GETICONTITLELOGFONT,sizeof(LOGFONT),&lf,0)) {
		VERIFY(m_fontPreviewList.CreateFontIndirect(&lf));
	} else {
		ASSERT(FALSE);
	}
	
	VERIFY(CreateImageListColor32(m_imageList,IDB_CHARTYPE,CHARTYPE_ICON_SIZE,CHARTYPE_ICON_SIZE,CHARTYPE_ICON_MASK));
	VERIFY(m_penPreviewGrid.CreatePen(PS_DOT,0,GetSysColor(COLOR_WINDOWTEXT)));
}

void InsertColumnToListCtrl(CListCtrl& list,int n,UINT nID,int nFormat,int nSize) {
	VERIFY(n == list.InsertColumn(n,LoadString(nID),nFormat,nSize,n));
}

void CharListDlg::SetupListListCtrl() {
	m_listList.SetImageList(&m_imageList,LVSIL_SMALL);
	m_listList.SetExtendedStyle(m_listList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	InsertColumnToListCtrl(m_listList,0,IDS_CLH_CHARNAME,LVCFMT_LEFT, 120);
	InsertColumnToListCtrl(m_listList,1,IDS_CLH_CHARTYPE,LVCFMT_LEFT,  70);
	InsertColumnToListCtrl(m_listList,2,IDS_CLH_CHARUSE ,LVCFMT_RIGHT, 50);
	InsertColumnToListCtrl(m_listList,3,IDS_CLH_CHARSIZE,LVCFMT_RIGHT, 70);
	InsertColumnToListCtrl(m_listList,4,IDS_CLH_CHARDESC,LVCFMT_LEFT, 100);
	m_listList.Register(this);
}

void CharListDlg::SetupPreviewListCtrl() {
	m_listPreview.SetExtendedStyle(m_listPreview.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listPreview.InsertColumn(0,NULL,LVCFMT_LEFT,0);
	m_listPreview.Register(this);
}

void CharListDlg::SetupPreviewStatic(void) {
	m_stPreview.SetListener(this);
}

void CharListDlg::OnSplitVMove() {
	AdjustSize();
	InvalidateView();
}

void CharListDlg::AdjustSize() {
	if (!m_bInit)
		return;
	
	CRect rcDlg;
	GetClientRect(rcDlg);
	CSize sz = rcDlg.Size();

	int m_nLowerTop = GetChildWindowBottom(m_splitter);
	int m_nUpperBottom = GetChildWindowTop(m_splitter);

	StretchChildWindowToRight(m_splitter,sz.cx);

	StretchChildWindowToRight(m_stPreview,sz.cx);
	StretchChildWindowToBottom(m_stPreview,m_nUpperBottom);

	MoveChildWindowToBottom  (m_itb,rcDlg.bottom);
	StretchChildWindowToRight(m_itb,sz.cx);

	int nListBottom = sz.cy - GetWindowHeight(m_itb);
	StretchChildWindowToRight (m_listList,sz.cx);
	MoveChildWindowToTop      (m_listList,m_nLowerTop);
	StretchChildWindowToBottom(m_listList,nListBottom);

	StretchChildWindowToRight (m_listPreview,sz.cx);
	MoveChildWindowToTop      (m_listPreview,m_nLowerTop);
	StretchChildWindowToBottom(m_listPreview,nListBottom);
	m_listPreview.SetColumnWidth(0,GetWindowWidth(m_listPreview));
}

void ListCtrllUnselectAll(CListCtrl& list) {
	ASSERT_VALID(&list);
	POSITION pos;
	while ((pos = list.GetFirstSelectedItemPosition()) != NULL) {
		int nSel = list.GetNextSelectedItem(pos);
		list.SetItemState(nSel,0,LVIS_SELECTED);
	}
	ASSERT(list.GetSelectedCount() == 0);
}

void ListCtrlSelectOne(CListCtrl& list,int nSel) {
	ASSERT_VALID(&list);
	if (nSel == LB_ERR)
		return;
	ASSERT(0 <= nSel && nSel < list.GetItemCount());
	list.SetItemState(nSel,LVIS_SELECTED,LVIS_SELECTED);
}

int ListCtrlGetSingleSelection(const CListCtrl& list) {
	ASSERT_VALID(&list);
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (pos == NULL) {
		return LB_ERR;
	}
	return list.GetNextSelectedItem(pos);
}

void SwitchActiveListCtrl(CListCtrl& listNew,CListCtrl& listOld) {
	ListCtrllUnselectAll(listNew);
	ListCtrlSelectOne(listNew,ListCtrlGetSingleSelection(listOld));
	listOld.ShowWindow(SW_HIDE);
	listNew.ShowWindow(SW_SHOW);
}

void CharListDlg::SwitchViewMode(VIEW_MODE vm,BOOL bForce) {
	if (!bForce && m_viewMode == vm)
		return;
	m_viewMode = vm;
	if (m_viewMode == VIEW_MODE_PREVIEW) {
		SwitchActiveListCtrl(m_listPreview,m_listList);
	}
	if (m_viewMode == VIEW_MODE_LIST) {
		SwitchActiveListCtrl(m_listList,m_listPreview);
	}

	AdjustSize();
	InvalidateView();
}

void CharListDlg::InvalidateView() {
	if (!m_bInit)
		return;

	m_stPreview.Invalidate();
	m_splitter.Invalidate();
	m_itb.Invalidate();
	m_listList.Invalidate();
	m_listPreview.Invalidate();
}

void CharListDlg::OnSize(UINT nType, int cx, int cy) {
	DialogBarDlg::OnSize(nType, cx, cy);

	AdjustSize();	
	InvalidateView();
}

void EnableWindowEx(CWnd* p,BOOL b) {
	if (p == NULL)
		return;
	BOOL bOld = p->IsWindowEnabled();
	if (bOld && b || !bOld && !b)
		return;
	p->EnableWindow(b);
}

void CharListDlg::OnDocumentActivated(CDocument* document) {
	DialogBarDlg::OnDocumentActivated(document);
	if (!m_bInit)
		return;

	m_document   = DYNAMIC_DOWNCAST(SpicydogDoc,document);
	if (m_document == NULL) {
		m_listList.   SetItemCountEx(0,LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
		m_listPreview.SetItemCountEx(0,LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
		m_charList = NULL;
	} else {
		m_charList = m_document->appli->charList.get();
	}
	BOOL bActive = ExistActiveDocument();
	EnableWindowEx(&m_listList,bActive);	
	EnableWindowEx(&m_listPreview,bActive);	
	EnableWindowEx(&m_itb,bActive);

	AdjustSize();
	InvalidateView();
}

void CharListDlg::OnDocumentUpdated(CDocument* document,LPARAM lHint,CObject*) {
	ASSERT(m_document == document);
	if (m_document == NULL) {
		return;
	}
	if (lHint == SpicydogDoc::UPDATED_PLAY) {
		return;
	}
	ASSERT_VALID(m_charList);
	ASSERT_VALID(m_document);

	int nCount = m_charList->GetCharCount();
	m_listList.SetItemCountEx(nCount,LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
	m_listPreview.SetItemCountEx(nCount,LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
	m_listList.Invalidate();
	m_listPreview.Invalidate();
	m_stPreview.Invalidate();
}

void UpdateList(CListCtrl& list) {
	const int num = list.GetItemCount();
	for (int n = 0;n < num;n++)
		list.Update(n);
}

void CharListDlg::OnNotify(DWORD dwNotifyCode,CDocument*,LPARAM,LPVOID) {
	if (dwNotifyCode != SpicydogDoc::EVENTNOTIFYCODE_BUILT)
		return;

	UpdateList(m_listList);
	UpdateList(m_listPreview);
}

void CharListDlg::OnPSPaint(class PreviewStatic& ps) {
	PaintMemDC dc(&ps);

	CRect rc;
	m_stPreview.GetWindowRect(rc);
	ScreenToClient(rc);
	
	COLORREF cr = ::GetSysColor(COLOR_APPWORKSPACE);
	dc.FillSolidRect(rc,cr);

	int n = GetSingleSelection();
	if (!ExistActiveDocument() || n == -1) {
		return;
	}

	const Char* charDraw = m_charList->GetChar(n);

	CharPreview preview;
	preview.dc = &dc;
	preview.bkgndColor = cr;
	rc.DeflateRect(PREVIEW_MARGIN_SIZE,PREVIEW_MARGIN_SIZE);
	preview.rect = rc;
	charDraw->DrawPreview(&preview);
}

const CListCtrl& CharListDlg::GetActiveList() const {
	if (m_viewMode == VIEW_MODE_LIST) {
		return m_listList;
	} else {
		ASSERT(m_viewMode == VIEW_MODE_PREVIEW);
		return m_listPreview;
	}
}

int CharListDlg::GetSelectedChars(std::list<Char*>& chars) {
	ASSERT(chars.size() == 0);
	ASSERT_VALID(m_document);
	ASSERT_VALID(m_charList);

	POSITION pos = GetActiveList().GetFirstSelectedItemPosition();
	while (pos != NULL) {
		int nSel = GetActiveList().GetNextSelectedItem(pos);
		ASSERT(0 <= nSel);
		chars.push_back(m_charList->GetChar(nSel));
	}

	return chars.size();
}

int CharListDlg::GetSingleSelection() const {
	return ListCtrlGetSingleSelection(GetActiveList());
}

BOOL CharListDlg::IsSingleSelected() const {
	return GetActiveList().GetSelectedCount() == 1;
}

BOOL CharListDlg::IsSelected() const {
	return 0 < GetActiveList().GetSelectedCount();
}

void CharListDlg::InvalidatePreview() {
	m_stPreview.Invalidate();
	m_stPreview.UpdateWindow();
}

void CharListDlg::OnUpdateExistActiveDocument(CCmdUI* pCmd) {
	pCmd->Enable(ExistActiveDocument());
}

void CharListDlg::OnInsCharMenu() {
	if (!ExistActiveDocument()) {
		return;
	}

	CRect rc;
	m_itb.GetButtonRect(0,rc);
	CPoint pt(rc.left,rc.bottom);
	m_itb.ClientToScreen(&pt);
	TrackNewPopupMenu(pt,GetTopLevelFrame(),IDM_CHARLIST,1);
}

AppliFrameWnd* CharListDlg::GetAppliFrameWnd() {
	CView* pView = GetActiveView();
	AppliFrameWnd* pMovieFrame = (AppliFrameWnd*)DYNAMIC_DOWNCAST(AppliFrameWnd,pView->GetParentFrame());
	ASSERT(pMovieFrame);
	return pMovieFrame;
}

BOOL CharListDlg::InsChar(Char* charIns) {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return FALSE;
	}

	ASSERT_VALID(charIns);
	ASSERT_VALID(m_document);
	ASSERT_VALID(m_charList);

	m_document->BeginEdit();
	if (charIns->GetCharType().guiModal) {
		if (!GetAppliFrameWnd()->OpenCharGUI(charIns)) {
			ASSERT_VALID(charIns);
			delete charIns;
			return FALSE;
		}
	}
	int pos = m_charList->GetCharCount();
	m_document->DoCmd(new InsCharCmd(m_charList,pos,charIns));
	m_document->UpdateAll(NULL);
	if (!charIns->GetCharType().guiModal) {
		VERIFY(GetAppliFrameWnd()->OpenCharGUI(charIns));
	}
	return TRUE;
}

void CharListDlg::ModChar(int nEdit) {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}
	ASSERT_VALID(m_document);
	ASSERT_VALID(m_charList);
	GetAppliFrameWnd()->OpenCharGUI(m_charList->GetChar(nEdit));
}

void CharListDlg::OnClickCharList(NMHDR*, LRESULT* pResult) {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}

	m_document->BeginEdit();
	InvalidatePreview();
	UpdateDialogControls(GetTopLevelFrame(),TRUE);
	*pResult = 0;
}

void CharListDlg::OnDblclkCharList(NMHDR*, LRESULT* pResult) {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}

	m_document->BeginEdit();
	if (IsSingleSelected()) {
		ModChar(GetSingleSelection());
	}
	*pResult = 0;
}

void CharListDlg::OnRclickCharList(NMHDR*, LRESULT* pResult) {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}
	*pResult = 0;
	m_document->BeginEdit();
	TrackNewPopupMenu(GetTopLevelFrame(),IDM_CHARLIST);
}

void CharListDlg::OnReturnCharList(NMHDR*, LRESULT* pResult) {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}

	m_document->BeginEdit();
	ModChar(GetSingleSelection());
	*pResult = 0;
}

void CharListDlg::OnSelChanged(NMHDR*, LRESULT* pResult) {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}

	m_document->BeginEdit();
	InvalidatePreview();
	*pResult = 0;
}

void CharListDlg::OnInsEmptyPictChar() {
	if (!ExistActiveDocument()) {
		ASSERT(FALSE);
		return;
	}

	ASSERT_VALID(m_document);
	m_document->BeginEdit();

	NewEmptyPictCharDlg dlg(AfxGetMainWnd());
	if (dlg.DoModal() != IDOK) {
		return;
	}

	PictChar* pPict = PictChar::newEmpty(
			dlg.GetSize(),

			m_document->appli->appliConf->bkgndColor);
	if (pPict != NULL) {
		pPict->SetName(m_document->charNamingRule.NewIncNumName(IDS_NEW_CHARNAME_PICT));
		InsChar(pPict);
	}
}

void CharListDlg::OnInsPictChar() {
	if (!ExistActiveDocument()) {
		return;
	}

	m_document->BeginEdit();
	ImportPictCharDlg dlg(AfxGetMainWnd());
	if (dlg.DoModal() != IDOK) {
		return;
	}

	BOOL bPathThrough = dlg.m_pathThrough;
	CWaitCursor wc;
	POSITION pos = dlg.GetStartPosition();
	while (pos != NULL) {
		CString strPath = dlg.GetNextPathName(pos);
		PictChar* pPict = bPathThrough ? 
								PictChar::newImportPathThrough(strPath) :
		                        PictChar::newImport(strPath);
		if (pPict == NULL) {
			UINT nFmt = bPathThrough ? 
							IDS_EN_PICT_INVALID_PATHTHROUGH_FORMAT : 
							IDS_EN_PICT_INVALID_IMPORT_FORMAT;
			AfxMessageBox(FormatString(nFmt,strPath));
			continue;
		}
		pPict->SetName(GetFileStripedName(GetFileName(strPath)));
		ASSERT_VALID(pPict);
		InsChar(pPict);
	}
}

void CharListDlg::OnInsTextChar() {
	if (!ExistActiveDocument()) {
		return;
	}
	m_document->BeginEdit();
	TextChar* text = new TextChar;
	ASSERT_VALID(text);
	if (InsChar(text)) {
		ASSERT_VALID(text);
		TextCharState state;
		text->GetState(state);
		text->SetName(TextChar::CreateNameByBody(state.text));
		m_document->UpdateAll(NULL);
	}
}

void CharListDlg::OnInsBtnChar() {
	if (!ExistActiveDocument()) {
		return;
	}
	m_document->BeginEdit();

	BtnChar* pBtn = new BtnChar;
	pBtn->SetName(m_document->charNamingRule.NewIncNumName(IDS_NEW_CHARNAME_BTN));
	InsChar(pBtn);
}

void CharListDlg::OnInsSndChar() {
	if (!ExistActiveDocument()) {
		return;
	}
	m_document->BeginEdit();

	CString strFilter;
	VERIFY(strFilter.LoadString(IDS_SND_FILTER));
	CFileDialog dlg(TRUE,NULL,NULL,0,strFilter,AfxGetMainWnd());
	if (dlg.DoModal() != IDOK)
		return;

	CFile file(dlg.GetPathName(),CFile::modeRead | CFile::shareDenyNone);
	GlobalAllocator alloc;
	alloc.Alloc(file.GetLength());
	GlobalAllocatorPtr ptr(alloc);

	file.Read(ptr,file.GetLength());

	SndChar* pSnd = new SndChar(alloc.Detach());
	pSnd->SetName(GetFileStripedName(dlg.GetPathName()));
	InsChar(pSnd);
}

void CharListDlg::OnRenameChar() {
	if (!ExistActiveDocument()) {
		return;
	}

	m_document->BeginEdit();

	if (!IsSingleSelected()) {
		return;
	}

	int nSel = GetSingleSelection();
	if (nSel == -1) {
		ASSERT(FALSE);
		return; 
	}
	ASSERT_VALID(m_charList);
	Char* pChar = m_charList->GetChar(nSel);
	if (pChar == NULL) { 
		ASSERT(FALSE);
		return;
	}
	ASSERT_VALID(pChar);

	CString str(pChar->GetName());
	RenCharName dlg(str,AfxGetMainWnd());
	if (dlg.DoModal() == IDOK) {
		m_document->DoCmd(new RenCharCmd(pChar,dlg.GetNewName()));
		m_document->UpdateAll(NULL);
	}
}

void CharListDlg::OnEditChar() {
	if (!ExistActiveDocument()) {
		return;
	}

	m_document->BeginEdit();

	if (!IsSelected())
		return;
	
	ModChar(GetSingleSelection());
}

void CharListDlg::OnAttachChar() {
	if (!ExistActiveDocument()) {
		return;
	}
	m_document->BeginEdit();
	if (!CanAttachChar()) {
		return;
	}

	Stage* stage;
	Layer* layer;
	Char*  chr;
	int    selectedLayer;
	int    selectedChar;
	int    selFrame;

	stage         = m_document->appli->GetActiveStage();;
	ASSERT_VALID(stage);
	selectedLayer = stage->GetSelLayer();
	ASSERT(selectedLayer != -1);
	layer         = stage->GetLayer(selectedLayer);
	ASSERT_VALID(layer);
	selectedChar  = GetSingleSelection();
	ASSERT(selectedChar != -1);
	chr           = m_charList->GetChar(selectedChar);
	ASSERT_VALID(chr);
	selFrame      = stage->GetSelFrame();

	CmdSet* cmds = new CmdSet(IDCMD_ATTACH_LAYER_CHAR);
	cmds->AddCommand(new AttachLayerCharCmd(layer,chr));
	if (selFrame != -1) {
		KeyFrame* insert = layer->GenerateKeyFrame(selFrame,FALSE);
		cmds->AddCommand(new InsKeyFrameCmd(layer,selFrame,insert));
	}

	m_document->DoCmd(cmds);
	m_document->UpdateAll(NULL);
}

void CharListDlg::OnUpdateAttachChar(CCmdUI* pCmd) {
	pCmd->Enable(CanAttachChar());
}

BOOL CharListDlg::CanAttachChar() {
	if (!ExistActiveDocument()) {
		return FALSE;
	}

	BOOL bSel = m_document->appli->GetActiveStage()->GetSelLayer() != -1 &&
		   IsSingleSelected();
	if (!bSel) {
		return FALSE;
	}
	int nChar = GetSingleSelection();
	Char* pChar = m_charList->GetChar(nChar);
	return pChar->GetCharType().canAttachLayer;
}

void CharListDlg::OnUpdateRequireSingleSelected(CCmdUI* pCmd) {
	pCmd->Enable(IsSingleSelected());
}

void CharListDlg::OnUpdateRequireSelected(CCmdUI* pCmd) {
	pCmd->Enable(IsSelected());
}

void CharListDlg::OnDupChar() {
	if (!ExistActiveDocument()) {
		return;
	}
	if (!IsSelected()) {
		return;
	}
	SpicydogDoc* document = m_document;
	ASSERT_VALID(document);
	CharList* charList = m_charList;
	document->BeginEdit();
	ASSERT_VALID(charList);
	int nSel = GetSingleSelection();
	ASSERT(0 <= nSel);
	const Char* pOrgChar = charList->GetChar(nSel);
	ASSERT_VALID(pOrgChar);
	Char* pDupChar = pOrgChar->Clone();
	ASSERT_VALID(pDupChar);
	pDupChar->SetName(document->charNamingRule.DupIncNumName(pOrgChar->GetName()));
	document->DoCmd(new DupCharCmd(charList,nSel+1,pDupChar));
	document->UpdateAll(NULL);
}


void CharListDlg::OnEditClear() {
	if (!ExistActiveDocument()) {
		return;
	}
	m_document->BeginEdit();

	if (!IsSelected()) {
		return;
	}
	m_document->DoCmd(new DelCharCmd(m_charList,GetSingleSelection()));
	m_document->UpdateAll(NULL);
}

void CharListDlg::OnEditCut() {
	if (!ExistActiveDocument()) {
		return;
	}
	m_document->BeginEdit();

	if (!IsSelected())
		return;

	OnEditCopy();

	m_document->DoCmd(new DelCharCmd(m_charList,GetSingleSelection()));
	m_document->UpdateAll(NULL);
}

void CharListDlg::OnEditCopy() {
	if (!ExistActiveDocument()) {
		return;
	}
	m_document->BeginEdit();

	int nSel = GetSingleSelection();
	if (nSel == -1)
		return;

	try
	{
		COleDataSource* pSrc = new COleDataSource;
		SubmitOle(pSrc,FALSE);
		pSrc->SetClipboard();
		pSrc->FlushClipboard();
	}
	catch(CException* pe)
	{
		VERIFY(pe->ReportError());
		pe->Delete();
	}
}

void CharListDlg::OnEditPaste() {
	if (!ExistActiveDocument()) {
		return;
	}
	m_document->BeginEdit();

	COleDataObject odo;
	if (odo.AttachClipboard())
		AcceptOle(&odo,FALSE);
	else
		ASSERT(FALSE);
}

void CharListDlg::OnUpdateEditPaste(CCmdUI* pcmd) {
	if (!ExistActiveDocument()) {
		return;
	}
	COleDataObject odo;
	if (odo.AttachClipboard()) {
		pcmd->Enable(m_charList->OleIsSupported(&odo));
	} else {
		pcmd->Enable(FALSE);
	}
}


void CharListDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT p) {
	ASSERT(nIDCtl == IDCL_PREVIEW);
	p->itemHeight = PREVIEW_LIST_HEIGHT; 
}

void CharListDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT p){
	ASSERT(nIDCtl == IDCL_PREVIEW);
	CDC* pDC = CDC::FromHandle(p->hDC);
	CRect rcItem = p->rcItem;

	MemDC dc(pDC,&rcItem); {
		BOOL bSel = p->itemState & ODS_SELECTED;
		int nSysClr = bSel ? COLOR_HIGHLIGHT : COLOR_WINDOW;
		dc.FillSolidRect(&p->rcItem,GetSysColor(nSysClr));
		
		CRect rcPreview(CPoint(rcItem.left + PREVIEW_LIST_MARGIN,
							   rcItem.top  + PREVIEW_LIST_MARGIN),
						CSize (PREVIEW_LIST_PREVIEW_HEIGHT,PREVIEW_LIST_PREVIEW_WIDTH));
		dc.FillSolidRect(rcPreview,::GetSysColor(COLOR_WINDOW));

		int nItem = p->itemID;
		if (nItem < m_charList->GetCharCount()) {
			CRgn rgn;
			BOOL bCliped = FALSE;
			if (rgn.CreateRectRgnIndirect(rcPreview)) {
				rgn.OffsetRgn(-dc.GetWindowOrg());
				dc.SelectClipRgn(&rgn);
				bCliped = TRUE;
			}
			CharPreview preview;
			preview.bkgndColor = ::GetSysColor(COLOR_WINDOW);
			preview.dc     = &dc;
			preview.rect      = rcPreview;
			preview.rect.DeflateRect(2,2);
			ASSERT_VALID(m_charList);
			const Char* pChar = m_charList->GetChar(nItem);
			ASSERT_VALID(pChar);
			pChar->DrawPreview(&preview);
			if (bCliped) {
				dc.SelectClipRgn(NULL);
			}

			CPoint ptIcon;
			ptIcon.x = rcPreview.right  - CHARTYPE_ICON_SIZE - PREVIEW_LIST_MARGIN;
			ptIcon.y = rcPreview.bottom - CHARTYPE_ICON_SIZE - PREVIEW_LIST_MARGIN;
			m_imageList.Draw(&dc,pChar->GetCharType().iconIndex,ptIcon,ILD_NORMAL);
			dc.FrameRect(rcPreview,&CBrush(GetSysColor(COLOR_WINDOWTEXT)));

			CFont* font =dc.SelectObject(&m_fontPreviewList);
			ASSERT_VALID(font);
			CRect rcText (rcPreview.right + PREVIEW_LIST_MARGIN,rcItem.top    + PREVIEW_LIST_MARGIN,
						  rcItem.right    - PREVIEW_LIST_MARGIN,rcItem.bottom - PREVIEW_LIST_MARGIN);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(::GetSysColor(bSel ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));
			dc.DrawText(pChar->GetName(),rcText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			VERIFY(dc.SelectObject(font) != NULL);
		} else {
			ASSERT(FALSE);
		}

		CPen* pen =dc.SelectObject(&m_penPreviewGrid);
		ASSERT_VALID(pen);
		dc.MoveTo(rcItem.left, rcItem.bottom-1);
		dc.LineTo(rcItem.right,rcItem.bottom-1);
		VERIFY(dc.SelectObject(pen) != NULL);
	}
}

void CharListDlg::OnPreviewView() {
	SwitchViewMode(VIEW_MODE_PREVIEW);
}

void CharListDlg::OnListView() {
	SwitchViewMode(VIEW_MODE_LIST);
}

void CharListDlg::OnUpdateView(CCmdUI* p) {
	p->Enable(ExistActiveDocument());
	if (p->m_nID == ID_CHARLIST_VIEW_LIST) {
		p->SetCheck(m_viewMode == VIEW_MODE_LIST);
	} else {
		ASSERT(p->m_nID == ID_CHARLIST_VIEW_PREVIEW);
		p->SetCheck(m_viewMode == VIEW_MODE_PREVIEW);
	}
}

void CharListDlg::OnGetCharInfo(NMHDR* pNMHDR, LRESULT* pResult) {
	ASSERT_VALID(m_charList);
	*pResult = 0;

	if (pNMHDR->idFrom == IDCL_PREVIEW || 
		!ExistActiveDocument() ||
		!m_listList.IsWindowVisible()) {
		return;
	}
	
	ASSERT(pNMHDR->idFrom == IDCL_LIST);
	LV_DISPINFO* p = (LV_DISPINFO*)pNMHDR;
	LV_ITEM& item= p->item;
	int iItemIndx= item.iItem;
	const Char* pChar = m_charList->GetChar(iItemIndx);
	
	if (item.mask & LVIF_IMAGE) {
		item.iImage = pChar->GetCharType().iconIndex;
	}
	
	if (item.mask & LVIF_TEXT) {
		CString str;
		switch(item.iSubItem){
		case 0:
			str = pChar->GetName();
			break;
		case 1:
			VERIFY(str.LoadString(pChar->GetCharType().displayName));
			break;
		case 2:
			str = FormatString(_T("%d"),pChar->GetUseCount());
			break;
		case 3:
			str = FormatString(_T("%d"),pChar->GetBuiltSize());
			break;
		case 4:
			str = pChar->GetLongInfo();
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		ASSERT(AfxIsValidAddress(item.pszText,item.cchTextMax));
		ASSERT(pChar->GetName().GetLength() < item.cchTextMax);
		lstrcpy(item.pszText,str);
	}
}





IMPLEMENT_DYNAMIC(RenCharName,CDialog)
BEGIN_MESSAGE_MAP(RenCharName,CDialog)
	//{{AFX_MSG_MAP(RenCharName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RenCharName::RenCharName(LPCTSTR ptsz,CWnd* pParent)
:	CDialog(IDD_REN_CHAR, pParent) {
	m_strName = ptsz;
}

void RenCharName::OnOK() {
	if (UpdateData(TRUE)) {
		CDialog::OnOK();
	}
}

CString RenCharName::GetNewName() const {
	return m_strName;
}

void RenCharName::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDCE_CHAR_NAME,m_strName);
	DDXUtility::DDV_TextLength(pDX,m_strName,2,32,
		                       IDCE_CHAR_NAME,IDCE_INVALID_CHARNAME);
}

BOOL CharListDlg::AcceptOle(COleDataObject* pOdo,BOOL bDnD,DROPEFFECT de,CPoint pt) {
	if (!ExistActiveDocument()) {
		return FALSE;
	}
	return m_charList->AcceptOle(DYNAMIC_DOWNCAST(SpicydogDoc,GetDocument()),pOdo,bDnD,de,pt);
}

/*
	D&D:	インラインキャラクタ情報
				1) プロセスID
				2) ドキュメントID
				3) キャラクタ数
				4) キャラクタポインタ * N
	C&P:	アウトラインキャラクタ情報
				1) シリアライズコンテキスト
				2) キャラクタ数
				3) キャラクタポインタ * N
			DIB画像
*/
BOOL CharListDlg::SubmitOle(COleDataSource* pSrc,BOOL bDnD) {
	if (!ExistActiveDocument()) {
		return FALSE;
	}

	ASSERT_VALID(pSrc);
	CharList* charList = m_charList;
	ASSERT_VALID(charList);

	std::list<Char*> chars;
	int nSel = GetSelectedChars(chars);
	if (nSel == 0)
		return FALSE;

	if (bDnD) {
		ASSERT_VALID(*chars.begin());
		pSrc->CacheGlobalData(
			CF_CHARS,
			charList->Data2Ole_CF_CHARS(DYNAMIC_DOWNCAST(SpicydogDoc,GetDocument()),chars));
	}
	return TRUE;
}
