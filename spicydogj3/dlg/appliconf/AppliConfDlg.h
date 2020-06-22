#pragma once

#include <memory>
#include <list>

BOOL AppliConfErrorBox(UINT);
class AppliConf;

class AppliConfDlg : public CPropertySheetEx {
public:
	enum MODE {MODE_WIZ,MODE_TAB};
private:
	typedef std::auto_ptr<CPropertyPageEx> PROPPAGEPTR;
	typedef std::list<PROPPAGEPTR> PROPPAGES;
	PROPPAGES  m_pages;
	MODE       m_mode;
	AppliConf& m_appconf;

public:
	AppliConfDlg(MODE,AppliConf&);
	~AppliConfDlg();
	static AppliConf& GetAppliConfRef(CPropertyPageEx*);
	static AppliConfDlg* GetAppliConfDlg(CPropertyPageEx*);
	static MODE GetAppliConfDlgMode(CPropertyPageEx*);

protected:
	void AddAppliConfPage(CPropertyPageEx*);
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};
