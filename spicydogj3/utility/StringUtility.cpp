#include "stdafx.h"
#include "StringUtility.h"

#if defined(_DEBUG)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif



CString LoadString(UINT n) {
	CString string;
	VERIFY(string.LoadString(n));
	return string;
}

CString FormatString(LPCTSTR ptszFormat,...) {
	va_list args;
	va_start(args, ptszFormat);

	CString str;
	str.FormatV(ptszFormat,args);

	return str;
}

CString FormatString(UINT nID,...) {
	CString strFormat;
	VERIFY(strFormat.LoadString(nID));

	va_list args;
	va_start(args, nID);

	CString str;
	str.FormatV(strFormat,args);
	return str;
}

CString FormatError(DWORD dwErrorCode) {
	LPVOID pvMessageBuffer;
	::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&pvMessageBuffer),
		0,
		NULL 
	);
	CString str(static_cast<LPTSTR>(pvMessageBuffer));
	::LocalFree(pvMessageBuffer);
	return str;
}

CString LoadStringResource(LPCTSTR lpName,LPCTSTR lpType) {
	return LoadStringResource(::AfxGetResourceHandle(),lpType,lpName);
}

CString LoadStringResource(UINT nID,LPCTSTR lpType) {
	return LoadStringResource(MAKEINTRESOURCE(nID),lpType);
}

CString LoadStringResource(HMODULE hMod,LPCTSTR lpType,LPCTSTR lpName) {
	return LoadStringResourceEx(hMod,lpType,lpName,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT));
}

CString LoadStringResourceEx(HMODULE hMod,LPCTSTR lpType,LPCTSTR lpName,WORD wLang) {
	ASSERT(hMod != NULL);
	ASSERT(((0xFFFF0000 & (DWORD)lpName) == 0) || AfxIsValidString(lpName));
	ASSERT(AfxIsValidString(lpType));

	HRSRC hr = FindResourceEx(hMod,lpType,lpName,wLang);
	if (hr == NULL) {
		TRACE("LoadStringResourceEx %d\n",GetLastError());
		ASSERT(FALSE);
		AfxThrowResourceException();
	}

	HGLOBAL hg = LoadResource(hMod,hr);
	if (hg == NULL)
	{
		TRACE("LoadStringResourceEx %d\n",GetLastError());
		ASSERT(FALSE);
		AfxThrowResourceException();
	}

	return CString(reinterpret_cast<LPCTSTR>(hg));
}


void Split(LPCTSTR _src,TCHAR delimit,CStringArray& result) {
	result.RemoveAll();

	CString src = _src;
	int start = 0;
	for (;;) {
		int end = src.Find(delimit,start);
		if (end == -1) {
			result.Add(src.Mid(start));
			break;
		}
		result.Add(src.Mid(start,end-start));
		start = end + 1;
	}
}

void SetClipText(LPCTSTR text) {
	ASSERT(AfxIsValidString(text));
	HGLOBAL hText;
	UINT    length;
	LPTSTR  clip;

	length = _tcslen(text) + 1;
	hText  = GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE,length * sizeof TCHAR);
	clip   = (LPTSTR)GlobalLock(hText);
		lstrcpy(clip,text);
	GlobalUnlock(hText);

	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hText);
	CloseClipboard();
}


