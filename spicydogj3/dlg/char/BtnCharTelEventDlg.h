#pragma once

#include "model/char/BtnChar.h"

class BtnCharTelEventDlg : public CDialog {
public:
	JphoneTelEvent telEvent;
	BtnCharTelEventDlg(const JphoneTelEvent* event);
	virtual ~BtnCharTelEventDlg();

private:
	void UpdateControls();
	afx_msg void OnChanged();
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual afx_msg void OnOK();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
