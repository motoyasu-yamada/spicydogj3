#pragma once

class SpicydogPref;
class PrefUploadPage : public CPropertyPage {
private:
	SpicydogPref* m_pref;
public:
	PrefUploadPage(SpicydogPref* pref);
	~PrefUploadPage();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnReset();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnChanged();
	void UpdateControls();
	DECLARE_MESSAGE_MAP()
};
