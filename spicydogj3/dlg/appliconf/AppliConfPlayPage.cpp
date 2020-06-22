#include "stdafx.h"
#include "resource.h"
#include "AppliConfPlayPage.h"
#include "dlg/ColorPaletteDlg.h"
#include "model/AppliConf.h"
#include "Spicydog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(AppliConfPlayPage, CPropertyPageEx)
	ON_BN_CLICKED(IDCC_LOOP,   OnChanged)
	ON_EN_CHANGE (IDCE_FPS,    OnChanged)
	ON_COMMAND   (IDCC_BKGND,  OnChanged)
	ON_COMMAND   (IDCC_BKFRAME,OnChanged)
	ON_MESSAGE(WM_CLRBOXEX_SELECT,OnColorBoxDown)
END_MESSAGE_MAP()

AppliConfPlayPage::AppliConfPlayPage(AppliConf* appliConf) 
:	CPropertyPageEx(IDD_APPLICONF_PLAY) {
	m_appliConf = appliConf;
}

AppliConfPlayPage::~AppliConfPlayPage() {
}

void AppliConfPlayPage::DoDataExchange(CDataExchange* pDX) {
	CPropertyPageEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCS_FPS, m_spinFps);
	ColorBoxEx::DDX_Control(pDX,IDCC_BKGND,  m_cbBkgnd,  IDB_CLRBOX_BKGND,RGB(0x00,0x80,0x80));
	ColorBoxEx::DDX_Control(pDX,IDCC_BKFRAME,m_cbBkframe,IDB_CLRBOX_FRAME,RGB(0x00,0x80,0x80));
	DDX_Text(pDX,IDCE_FPS,m_appliConf->frameRate);
	DDX_Check(pDX,IDCC_LOOP,m_appliConf->loopPlay);
	ColorBoxEx::DDX_COLORREF(pDX,IDCC_BKGND,  m_appliConf->bkgndColor);
	ColorBoxEx::DDX_COLORREF(pDX,IDCC_BKFRAME,m_appliConf->borderColor);
	DDV_MinMaxInt(pDX,m_appliConf->frameRate,FPS_INTERVAL_MIN,FPS_INTERVAL_MAX);
}

BOOL AppliConfPlayPage::OnInitDialog() {
	CPropertyPageEx::OnInitDialog();
	m_spinFps.SetRange(FPS_INTERVAL_MIN,FPS_INTERVAL_MAX);
	return TRUE;
}

LRESULT AppliConfPlayPage::OnColorBoxDown(WPARAM wparam,LPARAM lparam) {
	COLORREF* pCr = reinterpret_cast<COLORREF*>(lparam);
	CRect rc;
	if (wparam == IDCC_BKGND)
		m_cbBkgnd.GetWindowRect(rc);
	else {
		ASSERT(wparam == IDCC_BKFRAME);
		m_cbBkframe.GetWindowRect(rc);
	}

	ColorPaletteDlg dlg(*pCr,FALSE,rc.left,rc.bottom);
	if (dlg.DoModal() != IDOK) {
		return FALSE;
	}
	*pCr = dlg.GetColor();
	SetModified(TRUE);
	return TRUE;
}

void AppliConfPlayPage::OnChanged() {
	SetModified(TRUE);
}

BOOL AppliConfPlayPage::OnApply() {
	if (UpdateData(TRUE)) {
		CancelToClose();
		SpicydogApp::GetApp()->UpdateApplication();
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL AppliConfPlayPage::OnKillActive() {
	return UpdateData(TRUE);
}

void AppliConfPlayPage::OnOK() {
	if (UpdateData(TRUE)) {
		CPropertyPageEx::OnOK();
	}
}

BOOL AppliConfPlayPage::OnSetActive() {
	UpdateData(FALSE);
	return TRUE;
}
