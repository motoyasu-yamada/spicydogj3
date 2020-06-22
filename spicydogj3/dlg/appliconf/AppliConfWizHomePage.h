#pragma once
#include "utility/wnd/FontStatic.h"

class AppliConfWizHomePage : public CPropertyPageEx {
private:
	FontStatic m_staticTitle;
public:
	AppliConfWizHomePage();
	~AppliConfWizHomePage();

protected:
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnKillActive();
	DECLARE_MESSAGE_MAP()
};
