#include "stdafx.h"
#include "resource.h"
#include "AppliFrameWnd.h"

#include "model/char/Char.h"
#include "SpicydogDoc.h"
#include "utility/DocumentEvent.h"
#include "utility/ReenterLock.h"
#include "view/CharView.h"
#include "view/StageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(AppliFrameWnd, SECMDIChildWnd)
	ON_WM_CHILDACTIVATE()
	ON_COMMAND(ID_CLOSE_CHARVIEW,OnCloseCharView)
	ON_COMMAND(ID_CLOSE_ALL_CHARVIEW,OnCloseAllCharView)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_CHARVIEW,OnUpdateCloseCharView)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_ALL_CHARVIEW,OnUpdateCloseAllCharView)
END_MESSAGE_MAP()

//	コンストラクタ
IMPLEMENT_DYNCREATE(AppliFrameWnd, SECMDIChildWnd)
AppliFrameWnd::AppliFrameWnd()
:	m_ViewSwitchWnd(m_ViewSwitch) {
	ReenterLock::InitReenterLock(m_bEventHandling);
	m_ViewSwitch.AppendListener(this);
}				

AppliFrameWnd::~AppliFrameWnd()
{
}

//	キャラクタとビューの関連保持リストの比較子
class CHARVIEWCMP
{
private:
	const Char* m_pChar;
	const CView*     m_pView;
	BOOL  m_bFindByChar;
public:
	CHARVIEWCMP(const Char* pFind)
	:	m_pChar(pFind),
		m_pView(NULL),
		m_bFindByChar(TRUE)
	{
	}
	CHARVIEWCMP(const CView* pFind)
	:	m_pView(pFind),
		m_pChar(NULL),
		m_bFindByChar(FALSE)
	{
	}
	BOOL operator() (const AppliFrameWnd::CHARVIEWPTR& pcv) const
	{
		if (m_bFindByChar)
		{
			ASSERT(m_pView == NULL);
			ASSERT(m_pChar != NULL);
			
			const TOLMReferer<Char>& r = pcv->CharPtr;
			const Char* cmp = dynamic_cast<const Char*>(r.GetKeptOLMObject());
			return cmp == m_pChar;
		}
		else
		{
			ASSERT(m_pView != NULL);
			ASSERT(m_pChar == NULL);

			const CView* p = pcv->ViewPtr;
			return p == m_pView;
		}
	}
};

BOOL AppliFrameWnd::OpenCharModalGUI(SpicydogDoc* document,Char* aChar) {
	CRuntimeClass* guiClass = aChar->GetCharType().guiClass;
	ASSERT(guiClass->IsDerivedFrom(RUNTIME_CLASS(CharDialog)));

	std::auto_ptr<CharDialog> pdlg((CharDialog*)DYNAMIC_DOWNCAST(CharDialog,guiClass->CreateObject()));
	ASSERT_VALID(pdlg.get());
	pdlg->AttachChar(document,aChar);
	return pdlg->DoModal() == IDOK;
}

void AppliFrameWnd::OpenNewCharViewGUI(SpicydogDoc* document,Char* aChar) {
	ASSERT_VALID(document);
	ASSERT_VALID(aChar);
	const CharType& charType = aChar->GetCharType();

	CCreateContext context;
	context.m_pCurrentDoc   = document;
	context.m_pNewViewClass = charType.guiClass;
	
	UINT nID = AFX_IDW_PANE_FIRST + 1 + m_ViewSwitch.GetViewCount();

	CView* pView = (CView*) CreateView(&context,nID);
	CharView* pCharView = dynamic_cast<CharView*>(pView);
	if (pCharView != NULL) {
		pCharView->AttachChar(aChar);
		m_ViewSwitch.AppendListener(pCharView);
	}
	else
		ASSERT(FALSE);
	pView->SendMessage(WM_INITIALUPDATE);

	int nNewView = m_ViewSwitch.AppendView(pView,aChar->GetName(),TRUE,charType.iconIndex);
	m_ViewSwitch.ActivateView(nNewView);

	CHARVIEWPTR pcv(new CHARVIEW);
	pcv->CharPtr.ReferTo(aChar);
	pcv->CharPtr.AppendListener(this);
	pcv->ViewPtr = pView;
	m_CharViewList.push_back(pcv);
	m_ViewSwitchWnd.Update();
}

void AppliFrameWnd::ReopenCharViewGUI(Char* aChar) {
	const CHARVIEWPTR& pcv = *std::find_if(m_CharViewList.begin(),m_CharViewList.end(),CHARVIEWCMP(aChar));
	CView* pView = pcv->ViewPtr;
	ASSERT(pView != NULL);
	int nViews = m_ViewSwitch.GetViewCount();
	for (int nView = 0;nView < nViews;nView++) {
		ViewSwitchInfo info;
		m_ViewSwitch.GetViewInfo(nView,info);
		if (info.GetView() == pView) {
			m_ViewSwitch.ActivateView(nView);
			break;
		}
	}
	ASSERT(nView != nViews);
	m_ViewSwitchWnd.Update();
}

BOOL AppliFrameWnd::OpenCharGUI(Char* aChar) {
	ASSERT_VALID(aChar);
	const CharType& charType = aChar->GetCharType();
	SpicydogDoc* document = DYNAMIC_DOWNCAST(SpicydogDoc,GetActiveDocument());
	ASSERT_VALID(document);
	if (charType.guiModal) {
		return OpenCharModalGUI(document,aChar);
	}

	ASSERT(charType.guiClass->IsDerivedFrom(RUNTIME_CLASS(CView)));
	if (m_CharViewList.end() ==
		std::find_if(m_CharViewList.begin(),
		             m_CharViewList.end(),
					 CHARVIEWCMP(aChar))) {
		OpenNewCharViewGUI(document,aChar);
	} else {
		ReopenCharViewGUI(aChar);
	}
	return TRUE;
}

void AppliFrameWnd::OnRemovedOLMObject(COLMObject* p) {
	if (p == NULL) {
		ASSERT(FALSE);
		return;
	}
	ASSERT_VALID(p);

	Char* aChar = dynamic_cast<Char*>(p);
	if (aChar == NULL) {
		ASSERT(FALSE);
		return;
	}
	ASSERT_VALID(aChar);

	ReenterLock lock(m_bEventHandling);
	if (!lock.Lock()) {
		return;
	}

	CHARVIEWLIST::iterator i = std::find_if(m_CharViewList.begin(),m_CharViewList.end(),CHARVIEWCMP(aChar));
	if (i == m_CharViewList.end()) {
		ASSERT(FALSE);
		return;
	}

	const CHARVIEWPTR& pcv = *i;
	ASSERT(AfxIsValidAddress(pcv.get(),sizeof CHARVIEW));
	ASSERT_VALID(pcv->ViewPtr);
	ASSERT_KINDOF(CView,pcv->ViewPtr);

	ViewSwitchListener* pListerner = dynamic_cast<ViewSwitchListener*>(pcv->ViewPtr);
	if (pListerner == NULL)
		{ASSERT(FALSE);return;}
	ASSERT(AfxIsValidAddress(pListerner,sizeof ViewSwitchListener));

	m_ViewSwitch.RemoveListener(pListerner);
	pcv->CharPtr.RemoveListener(this);
	m_ViewSwitch.RemoveView(std::distance(m_CharViewList.begin(),i) + 1);
	
	pcv->ViewPtr->DestroyWindow();

	m_CharViewList.erase(i);
	m_ViewSwitchWnd.Update();
}

void AppliFrameWnd::OnUpdateCloseCharView(CCmdUI* pcmd) {
	ViewSwitchInfo info;
	int nActive = m_ViewSwitch.GetActiveView();
	m_ViewSwitch.GetViewInfo(nActive,info);
	pcmd->Enable(info.IsClosable());
}

void AppliFrameWnd::OnUpdateCloseAllCharView(CCmdUI* pcmd) {
	BOOL b = FALSE;
	int nc = m_ViewSwitch.GetViewCount();
	for (int n = 0;n < nc;n++)
	{
		ViewSwitchInfo info;
		m_ViewSwitch.GetViewInfo(n,info);
		if (info.IsClosable())
		{
			b = TRUE;
			break;
		}
	}

	pcmd->Enable(b);
}

void AppliFrameWnd::OnCloseCharView() {
	ViewSwitchInfo info;
	int nActive = m_ViewSwitch.GetActiveView();
	if (0 < nActive)
		m_ViewSwitch.ActivateView(nActive);

	m_ViewSwitch.GetViewInfo(nActive,info);
	if (info.IsClosable())
	{
		m_ViewSwitch.RemoveView(nActive);
		CView* pView = info.GetView();
		if (pView != NULL)
			pView->DestroyWindow();
		else
			ASSERT(FALSE);

		m_ViewSwitchWnd.Update();
	}
}

void AppliFrameWnd::OnCloseAllCharView() {
	for (;;)
	{
		BOOL bAllClosed = TRUE;
		int nc = m_ViewSwitch.GetViewCount();
		for (int n = 0;n < nc;n++)
		{
			ViewSwitchInfo info;
			m_ViewSwitch.GetViewInfo(n,info);
			if (info.IsClosable())
			{
				m_ViewSwitch.RemoveView(n);
				CView* pView = info.GetView();
				if (pView != NULL)
				{
					pView->DestroyWindow();
					bAllClosed = FALSE;
					break;
				}
				else
					ASSERT(FALSE);
			}
		}
		m_ViewSwitchWnd.Update();
		if (bAllClosed)
			break;
	}
}

void AppliFrameWnd::OnRemoveView(int,CView* pView) {
	if (pView == NULL)
		{ASSERT(FALSE);return;}
	ASSERT_KINDOF(CView,pView);

	ReenterLock lock(m_bEventHandling);
	if (!lock.Lock())
		return;

	CHARVIEWLIST::iterator i = std::find_if(m_CharViewList.begin(),m_CharViewList.end(),CHARVIEWCMP(pView));
	if (i == m_CharViewList.end())
		{ASSERT(FALSE);return;}

	const CHARVIEWPTR& pcv = *i;
	ASSERT(::AfxIsValidAddress(pcv.get(),sizeof CHARVIEW));
	TOLMReferer<Char>& r = (pcv->CharPtr);

	ViewSwitchListener* pListerner = dynamic_cast<ViewSwitchListener*>(pcv->ViewPtr);
	if (pListerner == NULL)
		{ASSERT(FALSE);return;}
	ASSERT(AfxIsValidAddress(pListerner,sizeof ViewSwitchListener));

	m_ViewSwitch.RemoveListener(pListerner);
	r.RemoveListener(this);

	m_CharViewList.erase(i);
}


BOOL AppliFrameWnd::PreCreateWindow(CREATESTRUCT& cs)  {
	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	
	return SECMDIChildWnd::PreCreateWindow(cs);
}

void AppliFrameWnd::ActivateFrame(int nCmdShow) {
    if (nCmdShow == -1)
        nCmdShow = SW_SHOWMAXIMIZED;

    SECMDIChildWnd::ActivateFrame(nCmdShow);
}

void AppliFrameWnd::OnUpdateFrameTitle(BOOL bAddToTitle) {
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);

	if ((GetStyle() & FWS_ADDTOTITLE) == 0)
		return;     // leave child window alone!

	CDocument* pDocument = GetActiveDocument();
	if (bAddToTitle)
	{
		TCHAR szText[256+_MAX_PATH];
		if (pDocument == NULL)
			lstrcpy(szText, m_strTitle);
		else
			lstrcpy(szText, pDocument->GetTitle());

		AfxSetWindowText(m_hWnd, szText);
	}
}

BOOL AppliFrameWnd::DestroyWindow()  {
	m_ViewSwitch.RemoveListener(this);
	return SECMDIChildWnd::DestroyWindow();
}

void AppliFrameWnd::RecalcLayout(BOOL bNotify) {
	if (m_bInRecalcLayout)
		return;

	m_bInRecalcLayout = TRUE;
	if (m_nIdleFlags & idleNotify)
		bNotify = TRUE;
	m_nIdleFlags &= ~(idleLayout|idleNotify);

	CRect rc;
	GetClientRect(rc);
	rc.top = VIEWSWITCHWND_HEIGHT;
	RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposExtra, &m_rectBorder,&rc);
	if (m_ViewSwitchWnd.GetSafeHwnd() != NULL)
	{
		m_ViewSwitchWnd.SetWindowPos(NULL,0,0,rc.Width(),VIEWSWITCHWND_HEIGHT,SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	m_bInRecalcLayout = FALSE;
}

BOOL AppliFrameWnd::CreateViewBarWnd() {
	ASSERT_VALID(this);
	return m_ViewSwitchWnd.Create(this);
}

BOOL AppliFrameWnd::CreateMovieView() {
	CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
	ASSERT(pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView)));
	ASSERT_VALID(pWnd);

	StageView* pView = (StageView*)DYNAMIC_DOWNCAST(StageView,pWnd);
	CString str;
	VERIFY(pView->GetViewCaption(str));
	m_ViewSwitch.AppendView(pView,str,FALSE,0);
	m_ViewSwitchWnd.Update();
	return TRUE;
}

void AppliFrameWnd::UpdateViewSwitchWnd()
{
	m_ViewSwitchWnd.Update();
}

BOOL AppliFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)  {
	ASSERT_POINTER(lpcs,CREATESTRUCT);
	ASSERT_POINTER(pContext,CCreateContext);
	ASSERT(lpcs     != NULL);
	ASSERT(pContext != NULL);
	
	if (!SECMDIChildWnd::OnCreateClient(lpcs, pContext)) {
		ASSERT(FALSE);
		return FALSE;
	}

	EnableDocking(CBRS_ALIGN_LEFT);
	EnableDocking(CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_BOTTOM);

	
	if (!CreateViewBarWnd()) {
		ASSERT(FALSE);
		return FALSE;
	}
	if (!CreateMovieView()) {
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}

/**
	子フレームとビューが存在するので、
	特殊な終了判定を行う。
*/
void AppliFrameWnd::OnClose() 
{
	if (m_lpfnCloseProc != NULL && !(*m_lpfnCloseProc)(this))
		return;

	// Note: only queries the active document
	CDocument* pDocument = GetActiveDocument();
	if (pDocument != NULL && !pDocument->CanCloseFrame(this))
	{
		// document can't close right now -- don't close it
		return;
	}
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL && pApp->m_pMainWnd == this)
	{
		// attempt to save all documents
		if (pDocument == NULL && !pApp->SaveAllModified())
			return;     // don't close it

		// hide the application's windows before closing all the documents
		pApp->HideApplication();

		// close all documents first
		pApp->CloseAllDocuments(FALSE);

		// don't exit if there are outstanding component objects
		if (!AfxOleCanExitApp())
		{
			// take user out of control of the app
			AfxOleSetUserCtrl(FALSE);

			// don't destroy the main window and close down just yet
			//  (there are outstanding component (OLE) objects)
			return;
		}

		// there are cases where destroying the documents may destroy the
		//  main window of the application.
		if (!afxContextIsDLL && pApp->m_pMainWnd == NULL)
		{
			AfxPostQuitMessage(0);
			return;
		}
	}

	// detect the case that this is the last frame on the document and
	// shut down with OnCloseDocument instead.
	if (pDocument != NULL && pDocument->m_bAutoDelete)
	{
		BOOL bOtherFrame = FALSE;
		POSITION pos = pDocument->GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = pDocument->GetNextView(pos);
			ASSERT_VALID(pView);
			//--- 以下 AppliFrameWndの特殊コード
			CFrameWnd* pViewFrame = pView->GetParentFrame();
			if (pViewFrame != this && pViewFrame->GetParentFrame() != this)
			{
				bOtherFrame = TRUE;
				break;
			}
			//--- 以上 AppliFrameWndの特殊コード
			//--- 通常 CFrameWnd::OnClose
			//	if (pView->GetParentFrame() != this && pViewFrame->GetParentFrame() != this)
			//	{
			//		bOtherFrame = TRUE;
			//		break;
			//	}
		}
		//if (!bOtherFrame)
		//{
		//	pDocument->OnCloseDocument();
		//	return;
		//}/

		// allow the document to cleanup before the window is destroyed
		pDocument->PreCloseFrame(this);
	}

	// then destroy the window
	DestroyWindow();
}

void AppliFrameWnd::OnActiveViewChanged(int nNew,int nOld) {
	if (nNew == nOld)
		return;
	CDocument* pDocBase = GetActiveDocument();
	ASSERT_VALID(pDocBase);
	SpicydogDoc* document = (pDocBase != NULL) ? DYNAMIC_DOWNCAST(SpicydogDoc,pDocBase): NULL;
	if (document != NULL)
		document->BeginEdit();
}

void AppliFrameWnd::OnChildActivate() {
	SECMDIChildWnd ::OnChildActivate();
	CView* pView = GetActiveView();
	if (pView == NULL)
		return;

	CDocument* document = pView->GetDocument();
	if (document == NULL)
		return;
	ASSERT_VALID(document);

	DocumentNotifier* pNotifier = DocumentNotifier::GetNotifier();
	pNotifier->ActivateDocument(document);
}
