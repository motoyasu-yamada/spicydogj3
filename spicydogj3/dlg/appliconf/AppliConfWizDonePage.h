#pragma once

class AppliConfWizDonePage : public CPropertyPageEx {
public:
	AppliConfWizDonePage();
	~AppliConfWizDonePage();

protected:
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnKillActive();
	DECLARE_MESSAGE_MAP()
};