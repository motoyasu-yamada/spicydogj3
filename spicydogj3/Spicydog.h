#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

class SpicydogApp : public CWinApp {
public:
	enum DDESTATE {ddestateNone,ddestateWaitOpen,ddestateOpened};
private:
	friend class SpicydogDocTempl;
	int      m_nFileNew;
	DDESTATE m_ddestate;

public:
	SpicydogApp();
	inline static SpicydogApp* GetApp() { return (SpicydogApp*)AfxGetApp(); }
	void UpdateApplication();

private:
	BOOL InitEnv();
	BOOL InitPref();
	BOOL InitMainWnd();
	void InitShell();
	BOOL ShowMainWnd();
	BOOL ProcessShell();
	void CraeteDocTemplates();
	afx_msg void OnAppAbout();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
#ifdef _DEBUG
	static void runTest();
#endif
	DECLARE_MESSAGE_MAP()
};