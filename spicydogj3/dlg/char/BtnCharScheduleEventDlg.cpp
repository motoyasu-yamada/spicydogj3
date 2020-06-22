#include "stdafx.h"
#include "resource.h"
#include "BtnCharScheduleEventDlg.h"
#include "utility/WndUtility.h"
#include "utility/DDXUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(BtnCharScheduleEventDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCC_RECEIVE_ALL,OnChanged)
	ON_EN_CHANGE (IDCE_CRITERIA,   OnChanged)
END_MESSAGE_MAP()

BtnCharScheduleEventDlg::BtnCharScheduleEventDlg(const JphoneScheduleEvent* event)
:	CDialog(IDD_BTN_CHAR_SCHEDULE_EVENT) {
	ASSERT_POINTER(event,JphoneScheduleEvent);
	ASSERT(event->useEvent);
	scheduleEvent = *event;
}

BtnCharScheduleEventDlg::~BtnCharScheduleEventDlg() {
}

void BtnCharScheduleEventDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX,IDCC_RECEIVE_ALL,scheduleEvent.receiveAll);
	DDX_Text(pDX,IDCE_CRITERIA,scheduleEvent.criteria);
	if (!scheduleEvent.receiveAll) {
		DDXUtility::DDV_TextLength(pDX,scheduleEvent.criteria,1,255,
			                       IDCE_CRITERIA,IDCS_ERR_INVALID_SCHEDULE_CRITERIA);
	}
}

void BtnCharScheduleEventDlg::OnOK() {
	if (UpdateData(TRUE)) {
		CDialog::OnOK();
	}
}

BOOL BtnCharScheduleEventDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	UpdateData(FALSE);
	UpdateControls();
	return TRUE;
}

void BtnCharScheduleEventDlg::OnChanged() {
	UpdateControls();
}

void BtnCharScheduleEventDlg::UpdateControls() {
	BOOL valid = DDXUtility::UpdateDataEx(this,TRUE);
	EnableDlgItem(this,IDOK,valid);
	EnableDlgItem(this,IDCE_CRITERIA,!scheduleEvent.receiveAll);
}

