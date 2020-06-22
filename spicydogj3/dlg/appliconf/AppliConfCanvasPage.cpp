#include "stdafx.h"
#include "resource.h"
#include "AppliConfCanvasPage.h"
#include "model/AppliConf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(AppliConfCanvasPage, CPropertyPageEx)
	ON_EN_CHANGE(IDCE_W, OnChanged)
	ON_EN_CHANGE(IDCE_H, OnChanged)
END_MESSAGE_MAP()

AppliConfCanvasPage::AppliConfCanvasPage(AppliConf* appliConf)
:	CPropertyPageEx(IDD_APPLICONF_CANVAS) {
	ASSERT_POINTER(appliConf,AppliConf);
	m_appliConf = appliConf;
}

AppliConfCanvasPage::~AppliConfCanvasPage() {
}

void AppliConfCanvasPage::DoDataExchange(CDataExchange* pDX) {
	CPropertyPageEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDCE_W, m_appliConf->stageSize.cx);
	DDX_Text(pDX, IDCE_H, m_appliConf->stageSize.cy);
}

BOOL AppliConfCanvasPage::OnApply() {
	if (UpdateData(TRUE)) {
		CancelToClose();
		return TRUE;
	} else {
		return FALSE;
	}
}

void AppliConfCanvasPage::OnOK() {
	if (UpdateData(TRUE)) {
		CPropertyPageEx::OnOK();
	}
}

BOOL AppliConfCanvasPage::OnSetActive() {
	UpdateData(FALSE);
	return CPropertyPageEx::OnSetActive();
}

BOOL AppliConfCanvasPage::OnKillActive(){
	if (UpdateData(TRUE)) {
		return CPropertyPageEx::OnKillActive();
	} else {
		return FALSE;
	}
}

void AppliConfCanvasPage::OnChanged() {
	SetModified(TRUE);
	UpdateControls();
}

void AppliConfCanvasPage::UpdateControls() {
}
