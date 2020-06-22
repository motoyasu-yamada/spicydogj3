#pragma once

class SpicydogPref;
class PrefEmuPage : public CPropertyPage {
private:
	SpicydogPref* m_pref;
public:
	PrefEmuPage(SpicydogPref* pref);
	~PrefEmuPage();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnReset();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnChanged();
	afx_msg void OnBrowser();
	afx_msg void OnDetect();
	DECLARE_MESSAGE_MAP()
};
