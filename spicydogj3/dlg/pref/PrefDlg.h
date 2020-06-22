#pragma once

#include <memory>
#include "SpicydogPref.h"

class PrefDlg : public CPropertySheet {
private:
	typedef std::auto_ptr<CPropertyPage> PAGEP;
	PAGEP m_pageEmu;
	PAGEP m_pageUpload;
	PAGEP m_pageEtc;
public:
	SpicydogPref editPref;
	PrefDlg(const SpicydogPref* pref,CWnd* parent);
	virtual ~PrefDlg();
private:
	DECLARE_MESSAGE_MAP()
};
