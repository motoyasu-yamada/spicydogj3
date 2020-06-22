#include "stdafx.h"
#include "resource.h"
#include "AppliConfWizHomePage.h"
#include "AppliConfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AppliConfWizHomePage::AppliConfWizHomePage()
:	CPropertyPageEx(IDD_APPLICONF_WIZHOME) {
}

AppliConfWizHomePage::~AppliConfWizHomePage()
{
}

void AppliConfWizHomePage::DoDataExchange(CDataExchange* pDX) {
	CPropertyPageEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDCS_TITLE,m_staticTitle);
}


BEGIN_MESSAGE_MAP(AppliConfWizHomePage, CPropertyPageEx)
END_MESSAGE_MAP()

BOOL AppliConfWizHomePage::OnSetActive() {
	AppliConfDlg::GetAppliConfDlg(this)->SetWizardButtons(PSWIZB_NEXT);
	return CPropertyPageEx::OnSetActive();
}

BOOL AppliConfWizHomePage::OnKillActive() {
	AppliConfDlg::GetAppliConfDlg(this)->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
	return CPropertyPageEx::OnKillActive();
}
