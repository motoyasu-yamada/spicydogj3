#pragma once
#include <list>

class AppliConf;
class AppliConfMachiukePage : public CPropertyPageEx {
private:
	CComboBox m_comboType;
	AppliConf* m_appliConf;
	std::list<CString> CLOCK_TYPES;
public:
	AppliConfMachiukePage(AppliConf*);
	~AppliConfMachiukePage();

protected:
	afx_msg void OnSelchangePlatforms();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnApply();
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	DECLARE_MESSAGE_MAP()
};
