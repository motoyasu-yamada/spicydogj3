#include "stdafx.h"
#include "resource.h"
#include "MainFrame.h"

#include "Spicydog.h"
#include "SpicydogDoc.H"
#include "SpicydogPref.h"
#include "dlg/HistoryDlg.h"
#include "dlg/build/BuildOutputDlg.h"
#include "dlg/build/BuildOutputDlg.h"
#include "dlg/char/CharListDlg.h"
#include "dlg/pref/GridEditDlg.h"
#include "dlg/pref/PrefDlg.h"
#include "dlg/stage/StagesDlg.h"
#include "dlg/stage/TimelineDlg.h"
#include "toolkit/secall.h"
#include "utility/FileUtility.h"
#include "utility/StringUtility.h"
#include "utility/WndUtility.h"
#include "wnd/BuildBar.h"
#include "wnd/ScrollViewEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PROGRESS_BUTTON(BTNID)	{BUTTON_CLASS(BuildBar),BTNID,TBBS_BUTTON, 0,0,0,0,0},
const LPCTSTR PATH_HELP = _T("index.chm");
const LPCTSTR ENTRYNAME_MAINFRAME = _T("MainFrame");
const LPCTSTR SECTIONNAME_WINDOWPLACEMENT = _T("WindowPlacement");
const LPCTSTR PROFILENAME_TOOLBARS = _T("ToolBars");
BEGIN_BUTTON_MAP(btnMap)
PROGRESS_BUTTON(ID_BUILD_BAR)
END_BUTTON_MAP()

struct CREATE_DSB {
	CRuntimeClass* m_pDlgClass;
	UINT m_nDialogID;
	UINT m_nTitleID;
	UINT m_nViewID;
	UINT m_nUpdateID;
	UINT m_nAccelID;
	UINT m_nDockStyle;
	UINT m_nFirstDock;
	double m_width;
	int    m_height;
};


UINT* toolbarGeneric;
UINT* toolbarModify;
UINT* toolbarPict;
UINT* toolbarInsChar;
UINT* toolbarPlay;
UINT* toolbarBuild;
UINT* toolbarView;

UINT toolbarGenericNum;
UINT toolbarModifyNum;
UINT toolbarPictNum;
UINT toolbarInsCharNum;
UINT toolbarPlayNum;
UINT toolbarBuildNum;
UINT toolbarViewNum;

const int DEFAULT_DIALOGBAR_WIDTH   = 200;
const int DEFAULT_DIALOGBAR_HEIGHT = 200;

CREATE_DSB CREATE_DSB_STAGES = {
	RUNTIME_CLASS(StagesDlg),IDD_STAGES,IDS_TITLE_STAGES,ID_VIEW_STAGES,ID_VIEW_STAGES,0,
	CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT,AFX_IDW_DOCKBAR_RIGHT,0.3,DEFAULT_DIALOGBAR_WIDTH
};

CREATE_DSB CREATE_DSB_DICTIONARY = {
	RUNTIME_CLASS(CharListDlg),IDD_CHARLIST,IDS_TITLE_CHARLIST,ID_VIEW_CHARLIST,ID_VIEW_CHARLIST,IDA_CHARLIST,
	CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT,AFX_IDW_DOCKBAR_RIGHT,0.7,DEFAULT_DIALOGBAR_WIDTH
};

CREATE_DSB CREATE_DSB_HISTORY = {
	RUNTIME_CLASS(HistoryDlg),IDD_HISTORY,IDS_TITLE_HISTORY,ID_VIEW_HISTORY,ID_VIEW_HISTORY,0,
	CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT,AFX_IDW_DOCKBAR_RIGHT,0,DEFAULT_DIALOGBAR_WIDTH
};

CREATE_DSB CREATE_DSB_BUILDOUTPUT = {
	RUNTIME_CLASS(BuildOutputDlg),IDD_BUILDOUTPUT,IDS_TITLE_BUILDOUTPUT,ID_VIEW_BUILDOUTPUT,ID_VIEW_BUILDOUTPUT,0,
	CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM,AFX_IDW_DOCKBAR_BOTTOM,0,DEFAULT_DIALOGBAR_HEIGHT	
};

CREATE_DSB CREATE_DSB_TIMELINE = {
	RUNTIME_CLASS(TimelineDlg),IDD_TIMELINE,IDS_TITLE_TIMELINE,ID_VIEW_TIMELINE,ID_VIEW_TIMELINE,IDA_TIMELINE,
	CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM,AFX_IDW_DOCKBAR_BOTTOM,1,DEFAULT_DIALOGBAR_HEIGHT
};

BEGIN_MESSAGE_MAP(MainFrame, SECMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_ACTIVATEAPP()
	ON_COMMAND(ID_PREF,OnPref)
	ON_COMMAND(ID_HELP,OnHelp)
	ON_COMMAND(ID_TBR_SWITCH,OnSwitchTbr)
	ON_COMMAND(ID_GRID_EDIT,OnEditGrid)
	ON_COMMAND(ID_DISPLAY_GRID,OnDisplayGrid)
	ON_COMMAND(ID_DISPLAY_CENTER_GRID,OnDisplayCenterGrid)
	ON_COMMAND(ID_DISPLAY_PIXEL_GRID,OnDisplayPixelGrid)
	ON_COMMAND(ID_VIEW_TOOLBAR_CUSTOMIZE, OnCustomize)
	ON_COMMAND_EX(ID_WWW_SPICYDOG,  OnWww)
	ON_COMMAND_EX(ID_WWW_SPICYSOFT, OnWww)
	ON_COMMAND_EX(ID_WWW_JPHONE,    OnWww)
	ON_COMMAND_EX(ID_VIEW_BUILDOUTPUT,OnBarCheckEx)
	ON_COMMAND_EX(ID_VIEW_STAGES,     OnBarCheckEx)
	ON_COMMAND_EX(ID_VIEW_CHARLIST,   OnBarCheckEx)
	ON_COMMAND_EX(ID_VIEW_TIMELINE,   OnBarCheckEx)
	ON_COMMAND_EX(ID_VIEW_HISTORY,    OnBarCheckEx)
	ON_COMMAND_EX(ID_VIEW_TOOLBOX,    OnBarCheckEx)
	ON_UPDATE_COMMAND_UI(ID_TBR_SWITCH,OnUpdateSwitchTbr)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BUILDOUTPUT,OnUpdateControlBarMenuEx)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STAGES,     OnUpdateControlBarMenuEx)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CHARLIST,   OnUpdateControlBarMenuEx)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TIMELINE,   OnUpdateControlBarMenuEx)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HISTORY,    OnUpdateControlBarMenuEx)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBOX,    OnUpdateControlBarMenuEx)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_GRID,OnUpdateDisplayGrid)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_CENTER_GRID,OnUpdateDisplayCenterGrid)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_PIXEL_GRID,OnUpdateDisplayPixelGrid)
END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_SEPARATOR,
	ID_SCROLLVIEWEX_MOUSE
};

const LPCTSTR PROFILE_PALETTE_BAR = _T("ToolPalette");
const DWORD COMMON_CTRL_STYLE = TBSTYLE_BUTTON | TBSTYLE_GROUP;
const DWORD COMMON_STYLE	  = WS_CHILD | WS_VISIBLE | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_TOP | CBRS_FLYBY | CBRS_SIZE_FIXED;
const int TOOLBAR_HEIGHT  = 32;
const int PANEWIDTH_MOUSE = 200;

#pragma warning(push)
#pragma warning(disable : 4355)
IMPLEMENT_DYNAMIC(MainFrame, SECMDIFrameWnd)
MainFrame::MainFrame()
:	m_bShowTbr(TRUE),
	m_bAppActive(TRUE),
	m_bInit(FALSE),
	m_ToolBarMgrPtr(new SECToolBarManager(this)),
	m_MenuBarPtr(new SECMDIMenuBar) {

	m_pControlBarManager = m_ToolBarMgrPtr.get();
	m_pMenuBar = NULL;//m_MenuBarPtr.get();
	EnableBmpMenus();
}
#pragma warning(pop)

MainFrame::~MainFrame() {
}

int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (SECMDIFrameWnd::OnCreate(lpCreateStruct) != 0)
		{ASSERT(FALSE);goto error;}

	if (!CreateStatusBar())
		{ASSERT(FALSE);goto error;}

	EnableDocking(CBRS_ALIGN_LEFT);
	EnableDocking(CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_BOTTOM);
 
	if (!CreateToolBox())
		{ASSERT(FALSE);goto error;}

	if (!CreateDialogSizingBar(CREATE_DSB_STAGES))
		{ASSERT(FALSE);goto error;}
	if (!CreateDialogSizingBar(CREATE_DSB_DICTIONARY))
		{ASSERT(FALSE);goto error;}
	if (!CreateDialogSizingBar(CREATE_DSB_TIMELINE))
		{ASSERT(FALSE);goto error;}
	if (!CreateDialogSizingBar(CREATE_DSB_BUILDOUTPUT))
		{ASSERT(FALSE);goto error;}
	if (!CreateDialogSizingBar(CREATE_DSB_HISTORY))
		{ASSERT(FALSE);goto error;}

	if (!CreateToolBars())
		{ASSERT(FALSE);goto error;}
	EnableDocking(CBRS_ALIGN_ANY);

    LoadBarState(PROFILENAME_TOOLBARS);
    m_pControlBarManager->LoadState(PROFILENAME_TOOLBARS);

	m_htmlHelp.Initialize(*GetDesktopWindow(),GetAppFilePath(PATH_HELP),NULL);
	m_bInit = TRUE;
	return 0;
error:
	return -1;
}

BOOL MainFrame::CreateToolBox() {
	if (!m_wndToolsBar.Create(this,ID_VIEW_TOOLBOX))
		{ASSERT(FALSE);return FALSE;}
	m_wndToolsBar.ShowWindow(SW_SHOW);
	m_wndToolsBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBarEx(&m_wndToolsBar, AFX_IDW_DOCKBAR_LEFT, 0, 0, (float)1.00,0);

	return TRUE;
}

void MainFrame::OnClose()  {
	m_htmlHelp.CloseAll();

	SaveWindowPlacement(this,SECTIONNAME_WINDOWPLACEMENT,ENTRYNAME_MAINFRAME);
	
	SaveBarState(PROFILENAME_TOOLBARS);
    m_pControlBarManager->SaveState(PROFILENAME_TOOLBARS);

	SECMDIFrameWnd::OnClose();
}

void SetStatusBarPaneWidth(SECStatusBar& wnd,UINT nID,int cx) {
	ASSERT_VALID(&wnd);
	ASSERT(0 < cx);
	int nIndex = wnd.CommandToIndex(nID);
	ASSERT(0 <= nIndex && nIndex < wnd.GetCount());

	UINT nDummy;
	UINT nStyle;
	int cxDummy;
	wnd.GetPaneInfo(nIndex,nDummy,nStyle,cxDummy);
	ASSERT(nDummy == nID);
	wnd.SetPaneInfo(nIndex,nID,nStyle,cx);
}

BOOL MainFrame::CreateStatusBar() {
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	dwStyle |= CBRS_BOTTOM;
	if (!m_wndStatusBar.Create(this,dwStyle,AFX_IDW_STATUS_BAR))
		{ASSERT(FALSE);return FALSE;}

	if (!m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
		{ASSERT(FALSE);return FALSE;}

	SetStatusBarPaneWidth(m_wndStatusBar,ID_SCROLLVIEWEX_MOUSE,PANEWIDTH_MOUSE);

	return TRUE;
}

BOOL MainFrame::CreateToolBars() {

	ASSERT(m_pControlBarManager != NULL);
	ASSERT_KINDOF(SECToolBarManager, m_pControlBarManager);
	SECToolBarManager* pMgr = (SECToolBarManager*)m_pControlBarManager;
	pMgr->EnableCoolLook(TRUE,CBRS_EX_COOL|CBRS_EX_BORDERSPACE|CBRS_EX_GRIPPER | CBRS_EX_FLATSTYLE);

	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDR_MAINFRAME));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_MODIFY));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_VIEW));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_PICT));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_INSCHAR));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_PLAY));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_BUILD));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_FRAME));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_LAYER));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_CHARLIST));
	pMgr->AddToolBarResource(MAKEINTRESOURCE(IDT_ETC));

	VERIFY(pMgr->LoadToolBarResource());	

	pMgr->SetButtonMap(btnMap);
	const UINT TOOLBAR_ID_BASE = AFX_IDW_TOOLBAR + 4;
	UINT nID = TOOLBAR_ID_BASE;

	pMgr->DefineDefaultToolBar(nID ++, 
								LoadString(IDS_TOOLBAR_TITLE_GENERIC),
								IDR_TOOLBAR_GENERIC,toolbarGenericNum,toolbarGeneric,
								CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM,
								AFX_IDW_DOCKBAR_TOP);
	UINT nIDPrev = TOOLBAR_ID_BASE;
	pMgr->DefineDefaultToolBar(nID ++, 
								LoadString(IDS_TOOLBAR_TITLE_MODIFY),
								IDR_TOOLBAR_MODIFY,toolbarModifyNum,toolbarModify,
								CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM,
								AFX_IDW_DOCKBAR_TOP,
								nIDPrev++,
								TRUE,
								FALSE);
	pMgr->DefineDefaultToolBar(nID ++, 
								LoadString(IDS_TOOLBAR_TITLE_PICT),
								IDR_TOOLBAR_PICT,toolbarPictNum,toolbarPict,
								CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM,
								AFX_IDW_DOCKBAR_TOP,
								nIDPrev++,
								TRUE,
								FALSE);
	pMgr->DefineDefaultToolBar(nID ++, 
								LoadString(IDS_TOOLBAR_TITLE_INSCHAR),
								IDR_TOOLBAR_INS_CHAR,toolbarInsCharNum,toolbarInsChar,
								CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM,
								AFX_IDW_DOCKBAR_TOP,
								nIDPrev++,
								TRUE,
								FALSE);
	pMgr->DefineDefaultToolBar(nID ++, 
								LoadString(IDS_TOOLBAR_TITLE_PLAY),
								IDR_TOOLBAR_PLAY,toolbarPlayNum,toolbarPlay,
								CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM,
								AFX_IDW_DOCKBAR_TOP,
								nIDPrev++,
								TRUE,
								FALSE);
	pMgr->DefineDefaultToolBar(nID ++, 
								LoadString(IDS_TOOLBAR_TITLE_BUILD),
								IDR_TOOLBAR_BUILD,toolbarBuildNum,toolbarBuild,
								CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM,
								AFX_IDW_DOCKBAR_TOP,
								nIDPrev++,
								TRUE,
								TRUE);
	pMgr->DefineDefaultToolBar(nID ++, 
								LoadString(IDS_TOOLBAR_TITLE_VIEW),
								IDR_TOOLBAR_VIEW,toolbarViewNum,toolbarView,
								CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM,
								AFX_IDW_DOCKBAR_TOP,
								nIDPrev++,
								TRUE,
								FALSE);
	return TRUE;
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !SECMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	return TRUE;
}

void MainFrame::OnUpdateControlBarMenuEx(CCmdUI* pCmdUI)
{
	CControlBar* pBar = GetControlBar(pCmdUI->m_nID);
	if (pBar != NULL)
	{
		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
		return;
	}
	pCmdUI->ContinueRouting();
}

BOOL MainFrame::OnBarCheckEx(UINT nID)
{
	CControlBar* pBar = GetControlBar(nID);
	if (pBar != NULL)
	{
		ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE);
		return TRUE;
	}
	return FALSE;
}

void MainFrame::OnPref() {
	CDocument* document = GetActiveDocument();
	if (document != NULL) {
		((SpicydogDoc*)DYNAMIC_DOWNCAST(SpicydogDoc,document))->BeginEdit();
	}

	SpicydogPref edit = ACTIVE_PREF;
	PrefDlg dlg(&edit,this);
	if (dlg.DoModal() == IDOK) {
		ACTIVE_PREF = dlg.editPref;
		SpicydogApp::GetApp()->UpdateApplication();
	}
}


BOOL MainFrame::OnWww(UINT nID) {
	CString strUrl;
	GetMessageString(nID,strUrl);
	if (strUrl.GetLength() == 0)
		return FALSE;

	VERIFY(OpenURL(strUrl));
	return TRUE;
}

void MainFrame::OnHelp() {
	m_htmlHelp.DisplayToc(NULL);
}


BOOL IsModalLooping(CWnd* pWnd) {
	if (pWnd != NULL) {
		ASSERT_VALID(pWnd);
		return (pWnd->m_nFlags & WF_MODALLOOP) != 0;
	} else {
		ASSERT(FALSE);
		return FALSE;
	}
}

void MainFrame::ShowOwnedTools(BOOL bShow)
{
	HWND hWnd = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
	while (hWnd != NULL)
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
		if (pWnd != NULL && m_hWnd != hWnd && !IsModalLooping(pWnd))
		{
			DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);

			if (!bShow && ((dwStyle & WS_VISIBLE) == WS_VISIBLE))
			{
				::ShowWindow(hWnd, SW_HIDE);
				pWnd->m_nFlags |= WF_TEMPHIDE;
			}
			else if (bShow && ((dwStyle & WS_VISIBLE) == 0) &&
				    (pWnd->m_nFlags & WF_TEMPHIDE))
			{
				::ShowWindow(hWnd, SW_SHOWNOACTIVATE);
				pWnd->m_nFlags &= ~WF_TEMPHIDE;
			}
		}
		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
}

void MainFrame::OnSwitchTbr()
{
	m_bShowTbr = !m_bShowTbr;
	ShowOwnedPopups(m_bShowTbr);
}

void MainFrame::OnUpdateSwitchTbr(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bShowTbr);
}

void FitWindowToStatusBar(CWnd* pWnd,CStatusBar* pBar,UINT nID)
{
	ASSERT_VALID(pWnd);
	ASSERT_VALID(pBar);

	CRect rc;
	pBar->GetStatusBarCtrl().GetRect(pBar->CommandToIndex(nID),rc);
	pWnd->SetWindowPos(NULL,rc.left + 1,rc.top + 1,rc.Width() - 2,rc.Height() - 2,SWP_NOZORDER | SWP_NOACTIVATE);
}

void UpdateAllViews()
{
	POSITION posTemp = AfxGetApp()->GetFirstDocTemplatePosition();
	while (posTemp != NULL)
	{
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(posTemp);
		POSITION posDoc = pTemplate->GetFirstDocPosition();
		while (posDoc != NULL)
		{
			CDocument* pDocument = pTemplate->GetNextDoc(posDoc);
			pDocument->UpdateAllViews(NULL);
		}
	}
}

void MainFrame::OnDisplayGrid() {
	ACTIVE_PREF.DisplayGrid = !ACTIVE_PREF.DisplayGrid;
	UpdateAllViews();
}

void MainFrame::OnDisplayCenterGrid() {
	ACTIVE_PREF.DisplayCenterGrid = !ACTIVE_PREF.DisplayCenterGrid;
	UpdateAllViews();
}

void MainFrame::OnDisplayPixelGrid() {
	ACTIVE_PREF.DisplayPixelGrid = !ACTIVE_PREF.DisplayPixelGrid;
	UpdateAllViews();
}

void MainFrame::OnUpdateDisplayGrid(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(ACTIVE_PREF.DisplayGrid ? 1:0);
}

void MainFrame::OnUpdateDisplayCenterGrid(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(ACTIVE_PREF.DisplayCenterGrid ? 1:0);
}

void MainFrame::OnUpdateDisplayPixelGrid(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(ACTIVE_PREF.DisplayPixelGrid ? 1:0);
}

void MainFrame::OnEditGrid() {
	GridEditDlg dlg;
	dlg.DoModal();
	UpdateAllViews();
}

void MainFrame::OnActivateApp(BOOL bActive, HTASK hTask) {
	SECMDIFrameWnd::OnActivateApp(bActive, hTask);
	m_bAppActive = bActive;
	OnActiveStateChanged();
}

void MainFrame::OnActiveStateChanged() {
	BOOL bShow = m_bAppActive;
	ShowOwnedTools(bShow);
}

void MainFrame::OnCustomize() {
	SECToolBarSheet    toolbarSheet;

	SECToolBarsPage    toolbarPage;
	toolbarPage.SetManager((SECToolBarManager*)m_pControlBarManager);
	toolbarSheet.AddPage(&toolbarPage);

	SECToolBarCmdPage  cmdPage(SECToolBarCmdPage::IDD);
	cmdPage.SetManager((SECToolBarManager*)m_pControlBarManager);
	cmdPage.DefineBtnGroup(LoadString(IDS_TOOLBAR_TITLE_GENERIC),toolbarGenericNum,toolbarGeneric);
	cmdPage.DefineBtnGroup(LoadString(IDS_TOOLBAR_TITLE_MODIFY), toolbarModifyNum, toolbarModify);
	cmdPage.DefineBtnGroup(LoadString(IDS_TOOLBAR_TITLE_PICT),   toolbarPictNum,   toolbarPict);
	cmdPage.DefineBtnGroup(LoadString(IDS_TOOLBAR_TITLE_INSCHAR),toolbarInsCharNum,toolbarInsChar);
	cmdPage.DefineBtnGroup(LoadString(IDS_TOOLBAR_TITLE_PLAY),   toolbarPlayNum,   toolbarPlay);
	cmdPage.DefineBtnGroup(LoadString(IDS_TOOLBAR_TITLE_BUILD),  toolbarBuildNum,  toolbarBuild);
	cmdPage.DefineBtnGroup(LoadString(IDS_TOOLBAR_TITLE_VIEW),   toolbarViewNum,   toolbarView);
	toolbarSheet.AddPage(&cmdPage);

	toolbarSheet.DoModal();
}

BOOL MainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHI) {
	if (!m_bInit)
		return SECMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHI);
	
	if (m_mgrDlgBar.OnCmdMsg1(nID, nCode, pExtra, pHI)) {
		return TRUE;
	}

	if (SECMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHI)) {
		return TRUE;
	}

	if (m_mgrDlgBar.OnCmdMsg2(nID, nCode, pExtra, pHI)) {
		return TRUE;
	}

	return FALSE;
}


BOOL MainFrame::OnCommand(WPARAM wParam, LPARAM lParam) {
	if (!m_bInit)
		return SECMDIFrameWnd::OnCommand(wParam, lParam);
	
	if (m_mgrDlgBar.OnCommand1(wParam,lParam)) {
		return TRUE;
	}

	if (SECMDIFrameWnd::OnCommand(wParam, lParam)) {
		return TRUE;
	}

	if (m_mgrDlgBar.OnCommand2(wParam,lParam)) {
		return TRUE;
	}

	return FALSE;
}

const int ACCELIDX_MAINFRAME = 0;
const int ACCELIDX_ACTIVE    = 1;
HACCEL MainFrame::GetActiveAccelerator() {
	m_accels.SetAccel(ACCELIDX_MAINFRAME,SECMDIFrameWnd::GetDefaultAccelerator());
	HACCEL ha = NULL;
	if (m_mgrDlgBar.IsAnyBarActive()) {
		ha  = m_mgrDlgBar.GetDefaultAccelerator();
	} else {
		ScrollViewEx* pActive = DYNAMIC_DOWNCAST(ScrollViewEx,GetActiveFrame()->GetActiveView());
		ASSERT(pActive != NULL);
		if (pActive != NULL) {
			ha = pActive->GetDefaultAccelerator();
		}
	}
	m_accels.SetAccel(ACCELIDX_ACTIVE,ha);
	HACCEL hac = m_accels.GetMergedAccel();
	TRACE("accels=%d\n",::GetAcceleratorTableSize(hac));
	return m_accels.GetMergedAccel();
}

BOOL MainFrame::CreateDialogSizingBar(const CREATE_DSB& dsb) {
	typedef std::auto_ptr<DialogSizingBar> BARPTR;
	BARPTR dialogSizingBar(new DialogSizingBar);
	
	UINT nStyle     = WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC;
	UINT dwExtStyle = CBRS_EX_COOL|CBRS_EX_BORDERSPACE|CBRS_EX_FLATSTYLE;
	if (!dialogSizingBar->Create(this,FormatString(dsb.m_nTitleID),nStyle,dwExtStyle,dsb.m_nViewID)) {
		ASSERT(FALSE);
		return FALSE;
	}
	dialogSizingBar->EnableDocking(dsb.m_nDockStyle);
	DockControlBarEx(dialogSizingBar.get(),dsb.m_nFirstDock,0,0,dsb.m_width,dsb.m_height);

	CDialog* pDlg = DYNAMIC_DOWNCAST(CDialog,dsb.m_pDlgClass->CreateObject());
	if (pDlg == NULL) {
		ASSERT(FALSE);
		return FALSE;
	}
	if (!pDlg->Create(dsb.m_nDialogID,dialogSizingBar.get())) {
		ASSERT(FALSE);
		delete pDlg;
		return FALSE;
	}
	pDlg->SetDlgCtrlID(dsb.m_nUpdateID);
	pDlg->ShowWindow(SW_SHOW);
	dialogSizingBar->AttachDialog(pDlg);
	if (dsb.m_nAccelID != 0) {
		dialogSizingBar->SetAccelerators(dsb.m_nAccelID);
	}

	m_mgrDlgBar.Append(dialogSizingBar.get());
	dialogSizingBar.release();
	return TRUE;
}