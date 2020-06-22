#include "stdafx.h"
#include "resource.h"
#include "BtnCharMailEventDlg.h"
#include "utility/WndUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(BtnCharMailEventDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
END_MESSAGE_MAP()

BtnCharMailEventDlg::BtnCharMailEventDlg(const JphoneMailEvent* event)
:	CDialog(IDD_BTN_CHAR_MAIL_EVENT) {
	ASSERT_POINTER(event,JphoneMailEvent);
	ASSERT(event->useEvent);
	mailEvent = *event;
}

BtnCharMailEventDlg::~BtnCharMailEventDlg() {
}

void BtnCharMailEventDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX,IDCC_MAIL_EVENT_STANDARD, mailEvent.standardMail);
	DDX_Check(pDX,IDCC_MAIL_EVENT_AUTO,     mailEvent.autoMail);
	DDX_Check(pDX,IDCC_MAIL_EVENT_EMERGENCY,mailEvent.emergencyMail);
	DDX_Check(pDX,IDCC_MAIL_EVENT_GREETING, mailEvent.greetingMail);
	DDX_Check(pDX,IDCC_MAIL_EVENT_RELAY,    mailEvent.relayMail);
	DDX_Check(pDX,IDCC_MAIL_EVENT_SUPER,    mailEvent.superMail);
	DDX_Check(pDX,IDCC_MAIL_EVENT_SKY,      mailEvent.skyMail);
}

BOOL BtnCharMailEventDlg::OnInitDialog() {
	UpdateData(FALSE);
	return TRUE;
}

void BtnCharMailEventDlg::OnOK() {
	if (UpdateData(TRUE)) {
		CDialog::OnOK();
		return;
	}
}

