#pragma once

#include <list>

class AppliConf;
class AppliConfTypePage : public CPropertyPageEx {
private:
	CComboBox m_comboType;
	AppliConf* m_appliConf;
	std::list<CString> APPLI_TYPE_KEYS;
	std::list<CString> APPLI_TYPE_NAMES;
public:
	AppliConfTypePage(AppliConf*);
	~AppliConfTypePage();

protected:
	void AddType(LPCTSTR key,UINT name);
	afx_msg void OnSelchangePlatforms();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnApply();
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	DECLARE_MESSAGE_MAP()
};
