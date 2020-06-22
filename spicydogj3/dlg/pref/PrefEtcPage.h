#pragma once

class SpicydogPref;
class PrefEtcPage : public CPropertyPage {
private:
	SpicydogPref* m_pref;
	CSpinButtonCtrl	m_spinMaxHistory;
public:
	PrefEtcPage(SpicydogPref* pref);
	~PrefEtcPage();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnReset();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnChanged();
	DECLARE_MESSAGE_MAP()
};
