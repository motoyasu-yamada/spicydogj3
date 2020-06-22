#include "stdafx.h"
#include "resource.h"
#include "BtnCharTelEventDlg.h"
#include "utility/WndUtility.h"
#include "utility/DDXUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(BtnCharTelEventDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCC_RECEIVE_ALL,OnChanged)
	ON_EN_CHANGE (IDCE_CRITERIA,   OnChanged)
END_MESSAGE_MAP()

BtnCharTelEventDlg::BtnCharTelEventDlg(const JphoneTelEvent* event)
:	CDialog(IDD_BTN_CHAR_TEL_EVENT) {
	ASSERT_POINTER(event,JphoneScheduleEvent);
	ASSERT(event->useEvent);
	telEvent = *event;
}

BtnCharTelEventDlg::~BtnCharTelEventDlg() {
}

void BtnCharTelEventDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX,IDCC_RECEIVE_ALL,telEvent.receiveAll);
	DDX_Text(pDX,IDCE_CRITERIA,telEvent.criteria);
	if (!telEvent.receiveAll) {
		DDXUtility::DDV_TextLength(pDX,telEvent.criteria,1,255,
			                       IDCE_CRITERIA,IDCS_INVALID_TEL_CRITERIA);
	}
}

void BtnCharTelEventDlg::OnOK() {
	if (UpdateData(TRUE)) {
		CDialog::OnOK();
	}
}

BOOL BtnCharTelEventDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	UpdateControls();
	return TRUE;
}

void BtnCharTelEventDlg::OnChanged() {
	UpdateControls();
}

void BtnCharTelEventDlg::UpdateControls() {
	BOOL valid = DDXUtility::UpdateDataEx(this,TRUE);
	EnableDlgItem(this,IDOK,valid);
	EnableDlgItem(this,IDCE_CRITERIA,!telEvent.receiveAll);
}

