#pragma once
#include "utility/wnd/ColorFaceButton.h"

class GridEditDlg : public CDialog {
private:
	ColorFaceButton	m_btnPixelGridColor;
	ColorFaceButton	m_btnGridColor;
	ColorFaceButton	m_btnCenterGridColor;
	CSpinButtonCtrl	m_spinGridInterval;

public:
	GridEditDlg(CWnd* pParent = NULL);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
