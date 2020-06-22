#include "stdafx.h"
#include "resource.h"
#include "UploadDlg.h"
#include "utility/DDXUtility.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef  THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(UploadDlg,CDialog)
	ON_BN_CLICKED(IDCC_UPLOAD,OnChanged)
END_MESSAGE_MAP()

UploadDlg::UploadDlg(CWnd* parent) :
	CDialog(IDD_UPLOAD,parent) {
}

void UploadDlg::OnOK() {
	if (UpdateData(TRUE)) {
		ACTIVE_PREF = m_pref;
		CDialog::OnOK();
	}
}

BOOL UploadDlg::OnInitDialog() {
	if (!CDialog::OnInitDialog()) {
		return FALSE;
	}
	m_pref = ACTIVE_PREF;
	UpdateData(FALSE);
	UpdateControls();
	return TRUE;
}

void UploadDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDCE_TO,m_pref.FwdMailTo);
	DDX_Text(pDX,IDCE_FROM,m_pref.FwdMailFrom);
	DDX_Text(pDX,IDCE_SMTP_SERVER,m_pref.FwdMailSMTPServer);
	DDX_Text(pDX,IDCE_SUBJECT,m_pref.FwdMailSubject);
	DDX_Text(pDX,IDCE_BODY,m_pref.FwdMailBody);
	DDX_Check(pDX,IDCC_UPLOAD,m_pref.FwdMailUse);

	if (m_pref.FwdMailUse) {
		DDXUtility::DDV_Emails(pDX,m_pref.FwdMailTo,  TRUE,
							   IDCE_TO,IDCS_ERR_INVALID_FWD_MAIL_TO);
		DDXUtility::DDV_Email (pDX,m_pref.FwdMailFrom,TRUE,
							   IDCE_FROM,IDCS_ERR_INVALID_FWD_MAIL_FROM);
		DDXUtility::DDV_Host  (pDX,m_pref.FwdMailSMTPServer,TRUE,
							   IDCE_SMTP_SERVER,IDCS_ERR_INVALID_FWD_MAIL_SMTP_SERVER);
	}
}

void UploadDlg::UpdateControls() {
	DDXUtility::UpdateDataEx(this,TRUE);
	BOOL enabled = m_pref.FwdMailUse;
	EnableDlgItem(this,IDCE_TO,  enabled);
	EnableDlgItem(this,IDCE_FROM,enabled);
	EnableDlgItem(this,IDCE_SMTP_SERVER,enabled);
	EnableDlgItem(this,IDCE_SUBJECT,    enabled);
	EnableDlgItem(this,IDCE_BODY,       enabled);
}

void UploadDlg::OnChanged() {
	UpdateControls();
}

