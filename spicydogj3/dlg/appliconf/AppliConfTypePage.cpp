#include "stdafx.h"
#include "resource.h"
#include "AppliConfTypePage.h"
#include "model/AppliConf.h"
#include "utility/DDXUtility.h"
#include "utility/StringUtility.h"
#include "sdom/sdomn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AppliConfTypePage::AppliConfTypePage(AppliConf* appliConf)
:	CPropertyPageEx(IDD_APPLICONF_TYPE) {
	m_appliConf = appliConf;
	ASSERT_POINTER(m_appliConf,AppliConf);
	AddType(ASDOMV_JPHONEF,  IDS_APPLI_TYPE_JPHONEF);
	AddType(ASDOMV_JPHONEFM, IDS_APPLI_TYPE_JPHONEFM);
	AddType(ASDOMV_JPHONEG,  IDS_APPLI_TYPE_JPHONEG);
	AddType(ASDOMV_JPHONEGM, IDS_APPLI_TYPE_JPHONEGM);
}

void AppliConfTypePage::AddType(LPCTSTR key,UINT name) {
	ASSERT(AfxIsValidString(key));
	APPLI_TYPE_KEYS.push_back(key);
	APPLI_TYPE_NAMES.push_back(LoadString(name));
}

AppliConfTypePage::~AppliConfTypePage() {
}

void AppliConfTypePage::DoDataExchange(CDataExchange* pDX) {
	CPropertyPageEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDCC_APPLI_TYPE,m_comboType);
	DDXUtility::DDX_CBValue(pDX,m_comboType,m_appliConf->appliType,APPLI_TYPE_KEYS,APPLI_TYPE_NAMES);
}

BEGIN_MESSAGE_MAP(AppliConfTypePage, CPropertyPageEx)
	ON_CBN_SELCHANGE(IDCC_APPLI_TYPE,OnSelchangePlatforms)
END_MESSAGE_MAP()

BOOL AppliConfTypePage::OnInitDialog() {
	CPropertyPageEx::OnInitDialog();
	UpdateData(FALSE);
	return TRUE;
}

void AppliConfTypePage::OnSelchangePlatforms() {
	SetModified(TRUE);
}

BOOL AppliConfTypePage::OnApply() {
	return UpdateData(TRUE);
}

void AppliConfTypePage::OnOK() {	
	if (UpdateData(TRUE)) {
		CPropertyPageEx::OnOK();
	}
}

BOOL AppliConfTypePage::OnSetActive() {
	UpdateData(FALSE);
	return CPropertyPageEx::OnSetActive();
}

BOOL AppliConfTypePage::OnKillActive() {
	return UpdateData(TRUE);
}
