#pragma once

/*	アプリケーションコンフィグ */
class SpicydogPref {
public:
	CString EmuPath;
	BOOL    FwdMailUse;
	CString FwdMailFrom;
	CString FwdMailTo;
	CString FwdMailSMTPServer;
	CString FwdMailSubject;
	CString FwdMailBody;
	
	int EtcMaxHistoryCount;

	BOOL DisplayGrid;
	COLORREF GridColor;
	int GridInterval;
	BOOL DisplayCenterGrid;
	COLORREF CenterGridColor;
	BOOL DisplayPixelGrid;
	COLORREF PixelGridColor;

	static void SaveConfig(CWinApp*);
	static void LoadConfig(CWinApp*);
};
extern SpicydogPref ACTIVE_PREF;
