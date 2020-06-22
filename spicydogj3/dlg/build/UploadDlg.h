#pragma once

#include "SpicydogPref.h"

class UploadDlg : public CDialog {
private:
	SpicydogPref m_pref;
public:
	UploadDlg(CWnd* parent);
private:
	void UpdateControls();
	afx_msg void OnChanged();
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
