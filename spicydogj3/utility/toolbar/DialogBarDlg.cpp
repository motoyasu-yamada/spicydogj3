#include "stdafx.h"
#include "DialogBarDlg.h"

#include "DialogSizingBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(DialogBarDlg, CDialog)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(DialogBarDlg,CDialog)
DialogBarDlg::DialogBarDlg()
:	CDialog(),
	m_bListening(FALSE),
	m_pActiveDocument(NULL) {
	// ‰½‚ÉŽg‚Á‚Ä‚é‚Ì ? SAFX::CReenterLock::InitReenterLock(m_bReenter);
}

DialogBarDlg::~DialogBarDlg() {
	if (m_bListening) {
		DocumentNotifier::GetNotifier()->RemoveListener(this);
	}
}

BOOL DialogBarDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	ASSERT(!m_bListening);
	DocumentNotifier::GetNotifier()->AddListener(this);
	m_bListening = TRUE;
	return TRUE;
}

void DialogBarDlg::OnClose() {
	ASSERT(m_bListening);
	DocumentNotifier::GetNotifier()->RemoveListener(this);
	m_bListening = FALSE;
	CDialog::OnClose();
}

void DialogBarDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DialogBarDlg)
	//}}AFX_DATA_MAP
}

void DialogBarDlg::OnCancel() {
}

void DialogBarDlg::OnOK() {
}

CDocument* DialogBarDlg::GetDocument() {
	ASSERT_VALID(m_pActiveDocument);
	return m_pActiveDocument;
}

BOOL DialogBarDlg::ExistActiveDocument() const {
	return m_pActiveDocument != NULL;
}

void DialogBarDlg::OnDocumentActivated(CDocument* p) {
	m_pActiveDocument = p;
}

BOOL DialogBarDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	if (!ExistActiveDocument())
		return FALSE;
	
	return GetDocument()->OnCmdMsg(nID,nCode,pExtra,pHandlerInfo);
}

BOOL DialogBarDlg::PreTranslateMessage(MSG* pMsg) {
	ASSERT(pMsg != NULL);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;
	
	DialogSizingBar* pParent = DYNAMIC_DOWNCAST(DialogSizingBar,GetParent());
	if (pParent != NULL && ((CDialog*)pParent)->PreTranslateMessage(pMsg)) {
		return TRUE;
	}

	if (::GetWindow(m_hWnd, GW_CHILD) == NULL)
		return FALSE;

	return PreTranslateInput(pMsg);
}
