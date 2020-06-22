#include "stdafx.h"
#include "resource.h"
#include "PrefEmuPage.h"
#include "SpicydogPref.h"
#include "utility/FolderDlg.h"
#include "utility/RegUtility.h"
#include "Jphone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(PrefEmuPage, CPropertyPage)
	ON_EN_CHANGE(IDCE_EMUPATH, OnChanged)
	ON_BN_CLICKED(IDCB_BROWSE,OnBrowser)
	ON_BN_CLICKED(IDCB_DETECT,OnDetect)
END_MESSAGE_MAP()

PrefEmuPage::PrefEmuPage(SpicydogPref* pref)
:	CPropertyPage(IDD_PREF_EMU) {
	ASSERT_POINTER(pref,SpicydogPref);
	m_pref = pref;
	ASSERT_POINTER(m_pref,SpicydogPref);
}

PrefEmuPage::~PrefEmuPage() {
}

void PrefEmuPage::DoDataExchange(CDataExchange* pDX) {
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX,IDCE_EMUPATH,m_pref->EmuPath);
}

BOOL PrefEmuPage::OnInitDialog() {
	CPropertyPage::OnInitDialog();
	UpdateData(TRUE);
	return TRUE;
}

void PrefEmuPage::OnReset() {
	UpdateData(FALSE);
	CPropertyPage::OnReset();
}

BOOL PrefEmuPage::OnApply() {
	UpdateData(TRUE);
	return CPropertyPage::OnApply();
}

void PrefEmuPage::OnChanged() {
	SetModified(TRUE);
}

void PrefEmuPage::OnBrowser() {
	UpdateData(TRUE);
	if (Jphone::BrowseEmuPath(m_pref->EmuPath)) {
		SetModified(TRUE);
		UpdateData(FALSE);
	}
}

void PrefEmuPage::OnDetect() {
	UpdateData(TRUE);
	if (Jphone::DetectEmuPath(m_pref->EmuPath)) {
		SetModified(TRUE);
		UpdateData(FALSE);
	} else {
		AfxMessageBox(IDS_ERR_CAN_NOT_DETECT_EMU_PATH,MB_OK | MB_ICONSTOP);
	}
}
