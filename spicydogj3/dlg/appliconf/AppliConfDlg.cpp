#include "stdafx.h"
#include "resource.h"
#include "AppliConfDlg.h"

#include "AppliConfTitlePage.h"
#include "AppliConfTypePage.h"
#include "AppliConfCanvasPage.h"
#include "AppliConfMachiukePage.h"
#include "AppliConfPlayPage.h"
#include "AppliConfWizHomePage.h"
#include "AppliConfWizDonePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(AppliConfDlg, CPropertySheetEx)
END_MESSAGE_MAP()

AppliConfDlg::AppliConfDlg(MODE m,AppliConf& appconf)
:	CPropertySheetEx(),
	m_mode(m),
	m_appconf(appconf)
{
	CBitmap bmp;
	if (!bmp.LoadBitmap(IDB_WIZHDR)) {
		ASSERT(FALSE);
		AfxThrowResourceException();
	}
	Construct(IDS_APPLICONF,NULL,0,bmp,NULL,bmp);
	bmp.Detach();

	if (m == MODE_TAB) {
		AddAppliConfPage(new AppliConfTitlePage(&appconf));
		AddAppliConfPage(new AppliConfTypePage(&appconf));
		AddAppliConfPage(new AppliConfPlayPage(&appconf));
		AddAppliConfPage(new AppliConfCanvasPage(&appconf));
		AddAppliConfPage(new AppliConfMachiukePage(&appconf));
	}
	if (m == MODE_WIZ) {
		SetWizardMode();
		m_psh.dwFlags &= (~PSH_WIZARD);
		m_psh.dwFlags |= PSH_WIZARD97;
		AddAppliConfPage(new AppliConfWizHomePage);
		AddAppliConfPage(new AppliConfTitlePage(&appconf));
		AddAppliConfPage(new AppliConfTypePage(&appconf));
		AddAppliConfPage(new AppliConfPlayPage(&appconf));
		AddAppliConfPage(new AppliConfCanvasPage(&appconf));
		AddAppliConfPage(new AppliConfWizDonePage);
	}
}

void AppliConfDlg::AddAppliConfPage(CPropertyPageEx* p) {
	m_pages.push_back(PROPPAGEPTR(p));
	AddPage(p);
}

AppliConfDlg::MODE AppliConfDlg::GetAppliConfDlgMode(CPropertyPageEx* pFromPage) {
	AppliConfDlg* pSheet = GetAppliConfDlg(pFromPage);
	ASSERT_VALID(pSheet);
	return pSheet->m_mode;
}

AppliConfDlg::~AppliConfDlg() {
}

void AppliConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertySheetEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AppliConfDlg)
	//}}AFX_DATA_MAP
}

AppliConf& AppliConfDlg::GetAppliConfRef(CPropertyPageEx* pFromPage)
{
	AppliConfDlg* pSheet = GetAppliConfDlg(pFromPage);
	ASSERT_VALID(pSheet);
	return pSheet->m_appconf;
}

AppliConfDlg* AppliConfDlg::GetAppliConfDlg(CPropertyPageEx* page) {
	ASSERT_VALID(page);
	CWnd* wnd = page->GetParent();
	ASSERT_VALID(wnd);
	ASSERT(wnd != NULL);
	ASSERT(_tcscmp(typeid(*wnd).name(),"class AppliConfDlg") == 0);
	AppliConfDlg* dlg = dynamic_cast<AppliConfDlg*>(wnd);
	ASSERT_VALID(dlg);
	return dlg;
}

BOOL AppliConfErrorBox(UINT nID) {
	AfxMessageBox(nID,MB_OK | MB_APPLMODAL | MB_ICONSTOP);
	return FALSE;
}


