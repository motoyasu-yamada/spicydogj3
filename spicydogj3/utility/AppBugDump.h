#pragma once
#include <imagehlp.h>

/** アプリケーション障害レポート */
class AppBugDump {
private:
	CString m_strSubject;
	CString m_strBody;

public:
	AppBugDump(PEXCEPTION_POINTERS);
	LPCTSTR GetSubject() const;
	LPCTSTR GetBody() const;

private:
	void DumpExceptionRecord(PEXCEPTION_RECORD);
	void DumpAppVer();
	void DumpDate();
	void DumpContext(CONTEXT*);
	void DumpRegister(CONTEXT*);
	void DumpSysInfo();
	void DumpAppFiles();
	void DumpFramePtr(CONTEXT*);
	void DumpStackDump(CONTEXT*);
	void DumpStackWalk(CONTEXT*);
	void DumpStackAddressed(HANDLE hProcess,CDWordArray& adwAddress);

	void DumpAppFilesHelper(int nDepth,LPCTSTR ptszPath);
	void DumpEol();
	void Dump(LPCTSTR);
	void DumpHeader(LPCTSTR);
	void DumpLn(LPCTSTR);
};