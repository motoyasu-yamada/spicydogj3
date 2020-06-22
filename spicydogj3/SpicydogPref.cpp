#include "stdafx.h"
#include "SpicydogPref.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

SpicydogPref ACTIVE_PREF;

class CConfigArchiver {
private:
	BOOL       m_bIsLoading;
	CWinApp*   m_pApp;
	CString    m_strCurrentSection;
public:
	CConfigArchiver(BOOL bLoad,CWinApp* pApp);
	virtual void SerializeConfig();

	void EnterSection(LPCTSTR);
	void SerializeInt(LPCTSTR,int&,int);
	void SerializeInt(LPCTSTR,long&,int);
	void SerializeInt(LPCTSTR,unsigned long&,int);
	void SerializeStr(LPCTSTR,CString&,LPCTSTR);
	void ExitSection();
}; // struct CConfigArchiver 

CConfigArchiver::CConfigArchiver(BOOL bLoad,CWinApp* pApp) :
	m_bIsLoading(bLoad),
	m_pApp(pApp) {
	ASSERT_VALID(m_pApp);
}

void CConfigArchiver::EnterSection(LPCTSTR p) {
	ASSERT(m_strCurrentSection.IsEmpty());
	m_strCurrentSection = p;
}

void CConfigArchiver::SerializeInt(LPCTSTR ptszEnt,int& v,int d) {
	ASSERT(!m_strCurrentSection.IsEmpty());
	if (m_bIsLoading)
		v = m_pApp->GetProfileInt(m_strCurrentSection,ptszEnt,d);
	else
		m_pApp->WriteProfileInt(m_strCurrentSection,ptszEnt,v);
}

void CConfigArchiver::SerializeInt(LPCTSTR ptszEnt,long& v,int d) {
	ASSERT(!m_strCurrentSection.IsEmpty());
	if (m_bIsLoading)
		v = m_pApp->GetProfileInt(m_strCurrentSection,ptszEnt,d);
	else
		m_pApp->WriteProfileInt(m_strCurrentSection,ptszEnt,v);
}

void CConfigArchiver::SerializeInt(LPCTSTR ptszEnt,unsigned long& v,int d) {
	ASSERT(!m_strCurrentSection.IsEmpty());
	if (m_bIsLoading)
		v = m_pApp->GetProfileInt(m_strCurrentSection,ptszEnt,d);
	else
		m_pApp->WriteProfileInt(m_strCurrentSection,ptszEnt,v);
}

void CConfigArchiver::SerializeStr(LPCTSTR ptszEnt,CString& strV,LPCTSTR ptszD) {
	ASSERT(!m_strCurrentSection.IsEmpty());
	if (m_bIsLoading)
		strV = m_pApp->GetProfileString(m_strCurrentSection,ptszEnt,ptszD);
	else
		m_pApp->WriteProfileString(m_strCurrentSection,ptszEnt,strV);
}

void CConfigArchiver::ExitSection() {
	ASSERT(!m_strCurrentSection.IsEmpty());
	m_strCurrentSection.Empty();
}

void CConfigArchiver::SerializeConfig() {
	EnterSection("Emu");
		SerializeStr("EmuPath",ACTIVE_PREF.EmuPath,"");
	ExitSection();

	EnterSection("FwdMai");
		SerializeInt("FwdMailUse",ACTIVE_PREF.FwdMailUse,FALSE);
		SerializeStr("From",      ACTIVE_PREF.FwdMailFrom,  "");
		SerializeStr("To",        ACTIVE_PREF.FwdMailTo,    "");
		SerializeStr("SMTPServer",ACTIVE_PREF.FwdMailSMTPServer,"");
		SerializeStr("Subject",   ACTIVE_PREF.FwdMailSubject,   "");
		SerializeStr("Body",      ACTIVE_PREF.FwdMailBody,      "");
	ExitSection();

	EnterSection("Grid");
		SerializeInt("DisplayGrid",       ACTIVE_PREF.DisplayGrid,      FALSE);
		SerializeInt("GridColor",         ACTIVE_PREF.GridColor,        RGB(0x90,0x90,0x90));
		SerializeInt("GridInterval",      ACTIVE_PREF.GridInterval,     10);
		SerializeInt("DisplayCenterGrid", ACTIVE_PREF.DisplayCenterGrid,FALSE);
		SerializeInt("CenterGridColor",   ACTIVE_PREF.CenterGridColor,  RGB(0x60,0x60,0x60));
		SerializeInt("DisplayPixelGrid",  ACTIVE_PREF.DisplayPixelGrid, FALSE);
		SerializeInt("PixelGridColor",    ACTIVE_PREF.PixelGridColor,   RGB(0xB0,0xB0,0xB0));
	ExitSection();

	EnterSection("Etc");
		SerializeInt("MaxHistoryCount",   ACTIVE_PREF.EtcMaxHistoryCount,  32);
	ExitSection();
}

void SpicydogPref::LoadConfig(CWinApp* pApp) {
	CConfigArchiver ar(TRUE,pApp);
	ar.SerializeConfig();
}

void SpicydogPref::SaveConfig(CWinApp* pApp) {
	CConfigArchiver ar(FALSE,pApp);
	ar.SerializeConfig();
}