#include "stdafx.h"
#include "resource.h"
#include "AppliConfMachiukePage.h"
#include "model/AppliConf.h"
#include "sdom/sdomn.h"
#include "utility/StringUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AppliConfMachiukePage::AppliConfMachiukePage(AppliConf* appliConf)
:	CPropertyPageEx(IDD_APPLICONF_MACHIUKE) {
	m_appliConf = appliConf;
	ASSERT_POINTER(m_appliConf,AppliConf);
	CLOCK_TYPES.push_back(ASDOMV_CLOCK_NONE);
	CLOCK_TYPES.push_back(ASDOMV_DIGITAL_CLOCK_SMALL);
	CLOCK_TYPES.push_back(ASDOMV_DIGITAL_CLOCK_LARGE);
}

AppliConfMachiukePage::~AppliConfMachiukePage() {
}

void AppliConfMachiukePage::DoDataExchange(CDataExchange* pDX) {
	CPropertyPageEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDCC_CLOCK,m_comboType);
	DDX_CBIndex(pDX,IDCC_CLOCK,m_appliConf->layerOverClock);
}

BEGIN_MESSAGE_MAP(AppliConfMachiukePage, CPropertyPageEx)
	ON_CBN_SELCHANGE(IDCC_CLOCK,OnSelchangePlatforms)
END_MESSAGE_MAP()

BOOL AppliConfMachiukePage::OnInitDialog() {
	CPropertyPageEx::OnInitDialog();
	m_comboType.AddString(LoadString(IDS_CLOCK_TYPE_CLOCK_NONE));
	m_comboType.AddString(LoadString(IDS_CLOCK_TYPE_DIGITAL_CLOCK_SMALL));
	m_comboType.AddString(LoadString(IDS_CLOCK_TYPE_DIGITAL_CLOCK_LARGE));
	UpdateData(FALSE);
	return TRUE;
}

void AppliConfMachiukePage::OnSelchangePlatforms() {
	SetModified(TRUE);
}

BOOL AppliConfMachiukePage::OnApply() {
	return UpdateData(TRUE);
}

void AppliConfMachiukePage::OnOK() {	
	if (UpdateData(TRUE)) {
		CPropertyPageEx::OnOK();
	}
}

BOOL AppliConfMachiukePage::OnSetActive() {
	UpdateData(FALSE);
	return CPropertyPageEx::OnSetActive();
}

BOOL AppliConfMachiukePage::OnKillActive() {
	return UpdateData(TRUE);
}
