#include "stdafx.h"
#include "NamingRule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString SplitName(LPCTSTR ptszText) {
	ASSERT(ptszText != NULL && AfxIsValidString(ptszText));
	LPCTSTR ptszStart = ptszText;
	int nLen = lstrlen(ptszStart);
	if (nLen == 0) {
		ASSERT(FALSE);
		return (LPCTSTR)NULL;
	}

	LPCTSTR ptszCurrent = ptszStart + nLen -1;
	if (!_istdigit(*ptszCurrent))
		return ptszStart;

	while (ptszStart < ptszCurrent && _istdigit(*ptszCurrent)){
		ptszCurrent = _tcsdec(ptszStart,ptszCurrent);
	}

	int nNameLen = nLen - lstrlen(_tcsinc(ptszCurrent));
	return CString(ptszStart,nNameLen);
}


NamingRule::NamingRule()
:	m_nNumber(1) {
}

LONG NamingRule::GetNewIncNumber() const {
#if defined(_MT)
	CSingleLock lock(&m_cs,TRUE);
#endif
	LONG n = m_nNumber++;
	return n;
}

CString NamingRule::NewIncNumName(LPCTSTR ptszFmt) const {
	CString str;
	str.Format(ptszFmt,GetNewIncNumber());
	return str;
}

CString NamingRule::NewIncNumName(UINT nResID) const {
	CString str;
	str.Format(nResID,GetNewIncNumber());
	return str;
}

CString NamingRule::DupIncNumName(LPCTSTR ptszPrg) const {
	CString str;
	str.Format(_T("%d"),GetNewIncNumber());
	return SplitName(ptszPrg) + str;
}

