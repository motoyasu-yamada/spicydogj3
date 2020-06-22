#pragma once

class AppliConf;
class AppliConfCanvasPage : public CPropertyPageEx {
private:
	AppliConf* m_appliConf;

public:
	AppliConfCanvasPage(AppliConf*);
	~AppliConfCanvasPage();

protected:
	void UpdateControls();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnApply();
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg void OnChanged();
	DECLARE_MESSAGE_MAP()
};

