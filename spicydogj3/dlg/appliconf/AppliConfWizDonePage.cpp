#include "stdafx.h"
#include "resource.h"
#include "AppliConfWizDonePage.h"
#include "AppliConfDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(AppliConfWizDonePage, CPropertyPageEx)
END_MESSAGE_MAP()

AppliConfWizDonePage::AppliConfWizDonePage()
:	CPropertyPageEx(IDD_APPLICONF_WIZDONE) {
}

AppliConfWizDonePage::~AppliConfWizDonePage() {
}

void AppliConfWizDonePage::DoDataExchange(CDataExchange* pDX) {
	CPropertyPageEx::DoDataExchange(pDX);
}

BOOL AppliConfWizDonePage::OnSetActive()  {
	AppliConfDlg::GetAppliConfDlg(this)->SetWizardButtons(PSWIZB_FINISH | PSWIZB_BACK);
	return CPropertyPageEx::OnSetActive();
}

BOOL AppliConfWizDonePage::OnKillActive() {
	AppliConfDlg::GetAppliConfDlg(this)->SetWizardButtons(PSWIZB_NEXT | PSWIZB_BACK);
	return CPropertyPageEx::OnKillActive();
}
