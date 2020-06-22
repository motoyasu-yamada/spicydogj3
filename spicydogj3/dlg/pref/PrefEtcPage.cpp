#include "stdafx.h"
#include "resource.h"
#include "PrefEtcPage.h"
#include "SpicydogPref.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(PrefEtcPage, CPropertyPage)
	ON_EN_CHANGE(IDCE_MAX_HISTORY, OnChanged)
END_MESSAGE_MAP()

PrefEtcPage::PrefEtcPage(SpicydogPref* pref)
:	CPropertyPage(IDD_PREF_ETC) {
	ASSERT_POINTER(pref,SpicydogPref);
	m_pref = pref;
	ASSERT_POINTER(m_pref,SpicydogPref);
}

PrefEtcPage::~PrefEtcPage() {
}

void PrefEtcPage::DoDataExchange(CDataExchange* pDX) {
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDCS_MAX_HISTORY,m_spinMaxHistory);
	DDX_Text(pDX, IDCE_MAX_HISTORY,m_pref->EtcMaxHistoryCount);
	DDV_MinMaxInt(pDX,m_pref->EtcMaxHistoryCount,16,255);
}

BOOL PrefEtcPage::OnInitDialog() {
	CPropertyPage::OnInitDialog();
	m_spinMaxHistory.SetRange(16,128);
	UpdateData(TRUE);
	return TRUE;
}

void PrefEtcPage::OnReset() {
	UpdateData(FALSE);
	CPropertyPage::OnReset();
}

BOOL PrefEtcPage::OnApply() {
	UpdateData(TRUE);
	return CPropertyPage::OnApply();
}

void PrefEtcPage::OnChanged() {
	SetModified(TRUE);
}
