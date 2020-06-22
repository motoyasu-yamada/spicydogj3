#pragma once

#include "model/char/BtnChar.h"

class BtnCharScheduleEventDlg : public CDialog {
public:
	JphoneScheduleEvent scheduleEvent;
	BtnCharScheduleEventDlg(const JphoneScheduleEvent*);
	virtual ~BtnCharScheduleEventDlg();

private:
	void UpdateControls();
	afx_msg void OnChanged();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};
