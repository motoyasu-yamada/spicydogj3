#include "stdafx.h"
#include "DDXUtility.h"
#include <afxinet.h>
#include "utility/FileUtility.h"
#include "utility/StringUtility.h"
#include "utility/UTF8.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const LPCTSTR FILE_NAME_INVALID_CHAR_LIST = _T("\\/:,;*?\"<>|");
BOOL SimpleFloatParse(LPCTSTR lpszText, double& d);
BOOL IsValidHttpURL(LPCTSTR);
BOOL IsAscii(LPCTSTR);
BOOL IsValidEmail(LPCTSTR);

BOOL DDXUtility::SLIENT_DATA_EXCHANGE = FALSE;

void DDXUtility::DDV_FilePath(CDataExchange* pDX,CString& path,UINT ctrl,UINT errmsg) {
	if (pDX->m_bSaveAndValidate) {
		if (!ExistsPath(path) || ExistsDirectory(path)) {
			Fail(pDX,ctrl,errmsg);
		}
	}
}

void DDXUtility::DDV_UTF8TextLength(
	CDataExchange* pDX,CString& sjis,int minLength,int maxLength,
	UINT ctrl,UINT errmsg) {

	ASSERT(minLength < maxLength);
	ASSERT(0 <= minLength);
	if (pDX->m_bSaveAndValidate) {
		CString utf = ConvertToUTF8(sjis);
		int length = utf.GetLength();
		if (!(minLength <= length && length <= maxLength)) {
			Fail(pDX,ctrl,errmsg);
		}
	}
}

void DDXUtility::DDV_TextLength(
	CDataExchange* pDX,CString& str1,int nMinLen,int nMaxLen,
	UINT ctrl,UINT errmsg) {

	if (pDX->m_bSaveAndValidate) {
		int nLen = str1.GetLength();
		if (!(nMinLen <= nLen && nLen <= nMaxLen)) {
			ASSERT(pDX->m_bSaveAndValidate);
			MessageBox(pDX,FormatString(errmsg,nMinLen,nMaxLen));
			ActivateCtrl(pDX,ctrl);
			pDX->Fail();
		}
	}
}

void DDXUtility::DDV_Email(CDataExchange* pDX,CString& str1,BOOL required,UINT ctrl,UINT errmsg) {
	if (pDX->m_bSaveAndValidate) {
		if (str1.IsEmpty() && !required)
			return;

		if (!IsValidEmail(str1)) {
			Fail(pDX,ctrl,errmsg);
		}
	}
}

void DDXUtility::DDV_Emails(CDataExchange* pDX,CString& mails,BOOL required,UINT ctrl,UINT errmsg) {
	if (pDX->m_bSaveAndValidate) {
		if (mails.IsEmpty() && !required) {
			return;
		}
		
		int start = 0;
		for(;;) {
			int end   = mails.Find(',',start);
			CString mail;
			if (end == -1) {
				mail = mails.Mid(start);
			} else {
				mail = mails.Mid(start,end - start);
			}
			mail.TrimLeft();
			mail.TrimRight();
			if (mail.IsEmpty() || !IsValidEmail(mail)) {
				Fail(pDX,ctrl,errmsg);
			}
			if (end == -1) {
				break;
			}
			start = end + 1;
		}
	}
}

BOOL IsValidHostChar(TCHAR c) {
	if (c == _T('.')) {
		return TRUE;
	}
	if (c == _T('-')) {
		return TRUE;
	}
	if (_istalnum(c)) {
		return TRUE;
	}
	return FALSE;
}

// . 3•¶ŽšˆÈã63•¶ŽšˆÈ“à -A-Za-z0-9
void DDXUtility::DDV_Host  (CDataExchange* pDX,CString& host,BOOL required,UINT ctrl,UINT errmsg) {
	if (pDX->m_bSaveAndValidate) {
		UINT length = host.GetLength();
		if (!(3 <= length && length <= 63)) {
			Fail(pDX,ctrl,errmsg);
		}
		for (UINT pos = 0;pos < length;pos++) {
			TCHAR c = host.GetAt(pos);
			if (!IsValidHostChar(c)) {
				Fail(pDX,ctrl,errmsg);
			}
		}
	}
}

void DDXUtility::DDV_HttpURL(
	CDataExchange* pDX,CString& url,BOOL required,
	UINT ctrl,UINT errmsg) {
	
	if (url.IsEmpty() && !required) {
		return;
	}

	if (!IsValidHttpURL(url)) {
		ASSERT(pDX->m_bSaveAndValidate);
		Fail(pDX,ctrl,errmsg);
	}
}


void DDXUtility::DDX_TextByPercent(CDataExchange* pDX, int nIDC, double& value) {
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

	if (pDX->m_bSaveAndValidate) {
		TCHAR szBuffer[32];
		::GetWindowText(hWndCtrl, szBuffer, _countof(szBuffer));

		if (!SimpleFloatParse(szBuffer,value)) {
			Fail(pDX,nIDC,AFX_IDP_PARSE_REAL);
		}
		value /= 100;
	} else {
		AfxSetWindowText(hWndCtrl,FormatString("%d",static_cast<int>(value*100)));
	}
}

void DDXUtility::DDV_MinMaxPercent(
	CDataExchange* pDX,double value,double minVal,double maxVal,
	UINT ctrl,UINT errmsg) {

	ASSERT(minVal <= maxVal);
	if (value < minVal || value > maxVal) {
		ASSERT(pDX->m_bSaveAndValidate);
		MessageBox(pDX,FormatString(errmsg,minVal,maxVal));
		ActivateCtrl(pDX,ctrl);
		pDX->Fail();
	}

}


BOOL DDXUtility::UpdateDataEx(CWnd* wnd,BOOL bSaveAndValidate) {
	ASSERT_VALID(wnd);
	CWnd* savedFocus = wnd->GetFocus();
	ASSERT_VALID(savedFocus);
	CDataExchange dx(wnd, bSaveAndValidate);

	//_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	//HWND hWndOldLockout = pThreadState->m_hLockoutNotifyWindow;
	//ASSERT(hWndOldLockout != wnd->m_hWnd);
	//pThreadState->m_hLockoutNotifyWindow = wnd->m_hWnd;

	BOOL bOK = FALSE;
	ASSERT(!SLIENT_DATA_EXCHANGE);
	SLIENT_DATA_EXCHANGE = TRUE;
	try {
		class HackWnd : public CWnd {
			friend BOOL DDXUtility::UpdateDataEx(CWnd* wnd,BOOL bSaveAndValidate);
		};
		((HackWnd*)wnd)->DoDataExchange(&dx);
		bOK = TRUE;
	} catch(CException* e) {
		ASSERT(!bOK);
		e->Delete();
	} catch(...) {
		ASSERT(FALSE);
	}
	ASSERT(SLIENT_DATA_EXCHANGE);
	SLIENT_DATA_EXCHANGE = FALSE;
	//pThreadState->m_hLockoutNotifyWindow = hWndOldLockout;
	//	CWnd* savedFocus = wnd->GetFocus();
	//
	if (savedFocus != NULL) {
		savedFocus->SetFocus();
	}
	return bOK;
}

void DDXUtility::DDX_CBValue(
	CDataExchange* pDX,
	CComboBox&     combo,
	CString&       value,
	const std::list<CString>& keys,
	const std::list<CString>& names) {
	ASSERT_VALID(&combo);
	int index;
	std::list<CString>::const_iterator i;

	if (pDX->m_bSaveAndValidate) {
		index = combo.GetCurSel();
		if (index != -1) {
			i = keys.begin();
			std::advance(i,index);
			value = *i;
		} else {
			ASSERT(FALSE);
		}
	} else {
		int count = combo.GetCount();
		if (count == 0) {
			i = names.begin();
			while (i != names.end()) {
				combo.AddString(*i++);
			}
		} 
		i = std::find(keys.begin(),keys.end(),value);
		if (i != keys.end()) {
			index = std::distance<std::list<CString>::const_iterator>(keys.begin(),i);
		} else {
			index = 0;
		}
		combo.SetCurSel(index);
	}
}


void DDXUtility::Fail(CDataExchange* pDX,UINT ctrl,UINT errmsg) {
	MessageBox(pDX,errmsg);
	ActivateCtrl(pDX,ctrl);
	pDX->Fail();
}

void DDXUtility::MessageBox(CDataExchange* pDX,UINT message) {
	if (SLIENT_DATA_EXCHANGE) {
		return;
	}
	AfxMessageBox(message,MB_OK,MB_OK | MB_ICONSTOP);
}

void DDXUtility::MessageBox(CDataExchange* pDX,LPCTSTR message) {
	if (SLIENT_DATA_EXCHANGE) {
		return;
	}
	AfxMessageBox(message,MB_OK,MB_OK | MB_ICONSTOP);
}

void DDXUtility::ActivateCtrl(CDataExchange* pDX,UINT ctrl) {
	if (SLIENT_DATA_EXCHANGE) {
		return;
	}
	CWnd* pCtrl = pDX->m_pDlgWnd->GetDlgItem(ctrl);
	ASSERT_VALID(pCtrl);
	pCtrl->SetFocus();
}

BOOL SimpleFloatParse(LPCTSTR lpszText, double& d) {
	ASSERT(lpszText != NULL);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	TCHAR chFirst = lpszText[0];
	d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	if (d == 0.0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	return TRUE;
}

BOOL IsValidEmail(LPCTSTR ptsz) {
	LPCTSTR ptszI = ptsz;
	BOOL bFindAt = FALSE;
	TCHAR tc;
	while ((tc = *ptszI) != _T('\0')) {
		if (!_istascii(tc))
			return FALSE;

		if (tc == _T('@'))
		{
			if (bFindAt)
				return FALSE;
			bFindAt = TRUE;
		}
		ptszI++;
	}
	return bFindAt;
}

BOOL IsValidHttpURL(LPCTSTR ptsz)
{
	DWORD dwServiceType;
	CString strServer;
	CString strObject;
	INTERNET_PORT nPort;

	if (!AfxParseURL(ptsz,dwServiceType,strServer,strObject,nPort))
		return FALSE;
	if (dwServiceType == AFX_INET_SERVICE_HTTP ||
		dwServiceType == AFX_INET_SERVICE_HTTPS)
		return TRUE;
	return FALSE;
}

BOOL IsAscii(LPCTSTR ptsz) {
	while (*ptsz == NULL) {
		if (!_istascii(*ptsz))
			return FALSE;
		ptsz++;
	}
	return TRUE;
}