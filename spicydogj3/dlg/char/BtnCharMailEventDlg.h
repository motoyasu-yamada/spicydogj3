#pragma once

#include "model/char/BtnChar.h"

class BtnCharMailEventDlg : public CDialog {
public:
	JphoneMailEvent mailEvent;
	BtnCharMailEventDlg(const JphoneMailEvent*);
	virtual ~BtnCharMailEventDlg();
private:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual afx_msg void OnOK();
	DECLARE_MESSAGE_MAP()
};