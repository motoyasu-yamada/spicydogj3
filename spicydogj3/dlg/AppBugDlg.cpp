#include "stdafx.h"
#include "resource.h"
#include "AppBugDlg.h"

#include "utility/AppBugDump.h"
#include "utility/FileUtility.h"
#include "utility/StringUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(AppBugDlg, CDialog)
	ON_BN_CLICKED(IDCB_OPEN_MAILER, OnOpenMailer)
	ON_BN_CLICKED(IDCB_WWW_SPICYDOG, OnWwwSpicydog)
END_MESSAGE_MAP()

AppBugDlg::AppBugDlg(AppBugDump& d)
:	CDialog(IDD_APPBUGDUMP,NULL) {
	m_strSubject = d.GetSubject();
	m_strBody = d.GetBody();
}

void AppBugDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCE_BODY, m_edit);
	DDX_Text(pDX, IDCE_BODY, m_strBody);
	DDX_Text(pDX, IDCS_SUBJECT, m_strSubject);
}

BOOL AppBugDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_edit.SetSel(0,-1);
	m_edit.Copy();
	
	return TRUE;
}

void AppBugDlg::OnOpenMailer() {
	CWaitCursor wc;
	OpenURL(LoadString(ID_MAIL_SPICYDOG));
}

void AppBugDlg::OnWwwSpicydog() {
	CWaitCursor wc;
	OpenURL(LoadString(ID_WWW_SPICYDOG));
}
