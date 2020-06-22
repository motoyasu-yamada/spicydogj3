#include "stdafx.h"
#include "RegUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString QueryRegValue(HKEY hKey,LPCTSTR ptszKeyPath,LPCTSTR ptszValueName,LPCTSTR ptszDefaultValue) {
	ASSERT(hKey != NULL);
	ASSERT(AfxIsValidString(ptszKeyPath));
	ASSERT(ptszDefaultValue == NULL || AfxIsValidString(ptszDefaultValue));
	ASSERT(ptszValueName    == NULL || AfxIsValidString(ptszValueName));

	CString str = ptszDefaultValue;
	CRegKey key;
	if (key.Create(hKey,ptszKeyPath) != ERROR_SUCCESS) {
		ASSERT(FALSE);
		return str;
	}

	DWORD len;
	if (key.QueryValue(NULL,ptszValueName,&len) != ERROR_SUCCESS) {
		ASSERT(FALSE);
		return str;
	}

	LRESULT r = key.QueryValue(str.GetBuffer(len),ptszValueName,&len);
	str.ReleaseBuffer();
	ASSERT(r == ERROR_SUCCESS);

	return str;
}

void WriteRegKeyValue(HKEY hKey,LPCTSTR ptszKeyPath,LPCTSTR ptszValue,LPCTSTR ptszValueName) {
	ASSERT(hKey != NULL);
	ASSERT(AfxIsValidString(ptszKeyPath));
	ASSERT(ptszValue     == NULL || AfxIsValidString(ptszValue));
	ASSERT(ptszValueName == NULL || AfxIsValidString(ptszValueName));

	CRegKey key;
	if (key.Create(hKey,ptszKeyPath) != ERROR_SUCCESS) {
		ASSERT(FALSE);
		return;
	}

	VERIFY(key.SetValue(ptszValue,ptszValueName) == ERROR_SUCCESS);
}