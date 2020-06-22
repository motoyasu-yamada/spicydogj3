#pragma once
#include "utility/wnd/ColorBox.h"
class AppliConf;
class AppliConfPlayPage : public CPropertyPageEx {
private:
	AppliConf* m_appliConf;
	CSpinButtonCtrl	m_spinFps;
	ColorBoxEx m_cbBkgnd;
	ColorBoxEx m_cbBkframe;
public:
	AppliConfPlayPage(AppliConf*);
	~AppliConfPlayPage();
protected:
	afx_msg LRESULT OnColorBoxDown(WPARAM,LPARAM);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnApply();
	virtual BOOL OnKillActive();
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	afx_msg void OnChanged();
	DECLARE_MESSAGE_MAP()
};
