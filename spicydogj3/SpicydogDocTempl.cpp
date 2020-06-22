#include "stdafx.h"
#include "SpicydogDocTempl.h"

#include "dlg/appliconf/AppliConfDlg.h"
#include "model/Appli.h"
#include "model/AppliConf.h"
#include "utility/DocumentEvent.h"
#include "SpicydogDoc.h"
#include "Spicydog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SpicydogDocTempl::SpicydogDocTempl(
	UINT nIDResource,
	CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass,
	CRuntimeClass* pViewClass)

:	CMultiDocTemplate(nIDResource,pDocClass,pFrameClass,pViewClass) {

}

CDocument* SpicydogDocTempl::OpenDocumentFile(LPCTSTR lpszPathName,BOOL bMakeVisible) {
	SpicydogApp* pApp = (SpicydogApp*)AfxGetApp();
	CFrameWnd* pFrame = NULL;
	BOOL bAutoDelete;
	SpicydogDoc* pDocument = DYNAMIC_DOWNCAST(SpicydogDoc,CreateNewDocument());
	if (pDocument == NULL) {
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		goto error;
	}
	ASSERT_VALID(pDocument);
	bAutoDelete = pDocument->m_bAutoDelete;
	pDocument->m_bAutoDelete = FALSE;

	if (lpszPathName == NULL) {
		AppliConf ac;
		AppliConfDlg dlg(AppliConfDlg::MODE_WIZ,ac);
		if (dlg.DoModal() == IDCANCEL) {
			goto error;
		}
		
		SetDefaultTitle(pDocument);
		if (!bMakeVisible)
			pDocument->m_bEmbedded = TRUE;

		if (!pDocument->OnNewDocument()) {
			ASSERT(FALSE);
			goto error;
		}
		pDocument->appli->appliConf->CopyFrom(&ac);
		m_nUntitledCount++;
	} else {
		CWaitCursor wait;
		if (!pDocument->OnOpenDocument(lpszPathName)) {
			ASSERT(FALSE);
			goto error;
		}
		pDocument->SetPathName(lpszPathName);
	}
	ASSERT_VALID(pDocument);

	pFrame = CreateNewFrame(pDocument, NULL);

	if (pFrame == NULL) {
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		goto error;
	}
	ASSERT_VALID(pFrame);

	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);
	DocumentNotifier::GetNotifier()->ForceActivateDocument(pDocument);
	pDocument->OnBuild();
	pDocument->m_bAutoDelete = bAutoDelete;
	if (pApp->m_ddestate == SpicydogApp::ddestateWaitOpen)
		pApp->m_ddestate = SpicydogApp::ddestateOpened;
	return pDocument;

error:
	if (pDocument != NULL) {
		delete pDocument;
		pDocument = NULL;
	}
	if (pApp->m_ddestate == SpicydogApp::ddestateWaitOpen)
		pApp->m_ddestate = SpicydogApp::ddestateOpened;
	return NULL;
}