#include "stdafx.h"
#include "resource.h"
#include "HttpPost.h"
#include <AfxInet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HttpPost::HttpPost()
:	m_bCommit(FALSE),
	m_pData(NULL)
{
	AddBoundary();
}

HttpPost::~HttpPost()
{
	if (m_pData != NULL)
	{
		ASSERT(m_bCommit);
		ASSERT_POINTER(m_pData,BYTE);
		free(m_pData);
		m_pData = NULL;
	}
}

CString HttpPost::GetHeader() const
{
	ASSERT(m_bCommit);
	return GetHeaderHelper();
}

LPVOID HttpPost::GetData() const
{
	ASSERT(m_bCommit);
	ASSERT_POINTER(m_pData,BYTE);
	return m_pData;
}

int HttpPost::GetDataLength() const
{
	ASSERT(m_bCommit);
	return m_nDataLength;
}

void HttpPost::Commit()
{
	ASSERT(!m_bCommit);
	ASSERT(m_pData == NULL);

	LPCTSTR TERM = _T("--\r\n");
	m_fileData.Write(TERM,lstrlen(TERM));
	TCHAR tcNull = _T('\0');
	m_fileData.Write(&tcNull,sizeof TCHAR);
	m_fileData.Flush();

	m_nDataLength = m_fileData.GetLength();
	m_pData = m_fileData.Detach();
	m_bCommit = TRUE;

	ASSERT(AfxIsValidAddress(m_pData,m_nDataLength));
	ASSERT(m_nDataLength != 0);
	ASSERT(m_bCommit);
}

void HttpPost::AddFormData(LPCTSTR ptszKey,LPCTSTR ptszValue) {
	ASSERT(AfxIsValidString(ptszKey));
	ASSERT(ptszValue == NULL || AfxIsValidString(ptszValue));
	ASSERT(!m_bCommit);
	LPCTSTR FORMAT = _T("\r\nContent-Disposition: form-data; name=\"%s\"\r\n"
		                "\r\n"
						"%s\r\n");

	CString strOutput;
	strOutput.Format(FORMAT,ptszKey,ptszValue);
	m_fileData.Write(strOutput,strOutput.GetLength());

	AddBoundary();
}

void HttpPost::AddFormFile(LPCTSTR ptszKey,LPCTSTR ptszName,LPCTSTR ptszPath,LPCTSTR ptszContentType) {
	ASSERT(AfxIsValidString(ptszKey));
	ASSERT(AfxIsValidString(ptszName));
	ASSERT(AfxIsValidString(ptszPath));
	ASSERT(AfxIsValidString(ptszContentType));
	ASSERT(!m_bCommit);

	LPCTSTR FORMAT = _T("\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n"
					    "Content-Type: %s\r\n"
						"\r\n");
	CString strOutput;
	strOutput.Format(FORMAT,ptszKey,ptszName,ptszContentType);
	m_fileData.Write(strOutput,strOutput.GetLength());

	{
		CFile file(ptszPath,CFile::modeRead);
		DWORD nLen = file.GetLength();
		CArray<BYTE,BYTE> ab;
		ab.SetSize(nLen);
		VERIFY(file.Read(ab.GetData(),nLen) == nLen);

		m_fileData.Write(ab.GetData(),nLen);
	}
	strOutput = _T("\r\n");
	m_fileData.Write(strOutput,strOutput.GetLength());
	AddBoundary();
}

void HttpPost::AddBoundary()
{
	CString strBoundary = GetBoundaryHelper();
	strBoundary = _T("--") + strBoundary;
	m_fileData.Write(static_cast<LPCTSTR>(strBoundary),strBoundary.GetLength());
}

CString HttpPost::GetHeaderHelper() const
{
	return _T("Content-Type: multipart/form-data; boundary=") + GetBoundaryHelper();
}

CString HttpPost::GetBoundaryHelper() const
{
	return _T("-----------------------------7d1d71d602da");
}

