#include "stdafx.h"
#include "resource.h"
#include "PrefDlg.h"
#include "PrefEtcPage.h"
#include "PrefUploadPage.h"
#include "PrefEmuPage.h"
#include "SpicydogPref.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(PrefDlg, CPropertySheet)
END_MESSAGE_MAP()

PrefDlg::PrefDlg(const SpicydogPref* pref,CWnd* parent)
	:CPropertySheet(IDS_DN_PREF, parent,0) {
	ASSERT_POINTER(pref,SpicydogPref);
	editPref = *pref;

	m_pageEmu     = PAGEP(new PrefEmuPage(&editPref));
	m_pageUpload  = PAGEP(new PrefUploadPage(&editPref));
	m_pageEtc     = PAGEP(new PrefEtcPage(&editPref));
	AddPage(m_pageEmu.get());
	AddPage(m_pageUpload.get());
	AddPage(m_pageEtc.get());
}

PrefDlg::~PrefDlg() {
}
