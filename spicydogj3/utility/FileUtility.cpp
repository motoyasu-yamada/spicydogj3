#include "stdafx.h"
#include "FileUtility.h"

#include <malloc.h>

#if defined(_DEBUG)
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"version.lib")

UINT AFXAPI AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);
void AFXAPI AfxGetRoot(LPCTSTR lpszPath, CString& strRoot);

CString GetCurrentDirectory() {
	CString str;
	DWORD length;
	BOOL  result;

	length = ::GetCurrentDirectory(0,NULL);
	if (length == 0) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}

	result = ::GetCurrentDirectory(length,str.GetBuffer(length)) != 0;
	str.ReleaseBuffer();
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}
	return str;
}

CString GetAppExePath() {
	CString str;
	DWORD   length;
	BOOL    result;

	length = MAX_PATH * 10;
	result = ::GetModuleFileName(NULL,str.GetBuffer(length),length) != 0;
	str.ReleaseBuffer();
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}

	return str;
}

CString GetAppRootPath() {
	return GetFileParent(GetAppExePath());
}

CString GetAppFilePath(LPCTSTR ptszPath) {
	return CatPath(GetAppRootPath(),ptszPath);
}

CString CatPath(LPCTSTR lptszLHS,LPCTSTR lptszRHS) {
	ASSERT(lptszLHS != NULL && lptszRHS != NULL);
	ASSERT(AfxIsValidString(lptszLHS));
	ASSERT(AfxIsValidString(lptszRHS));

	CString str = lptszLHS;
	if (str.Right(1) == "\\") {
		str += lptszRHS;
	} else {
		str += "\\";
		str += lptszRHS;
	}
	return str;
}


CString GetFileParent(LPCTSTR ptszPath) {
	ASSERT(ptszPath != NULL && AfxIsValidString(ptszPath));

	CString str(ptszPath);
	if (str.Right(1) == '\\') {
		str = str.Left(str.GetLength() - 1);
	}

	int n = str.ReverseFind('\\');
	if (n == -1) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}

	str = str.Left(n);
	return str;
}

UINT GetFileLength(LPCTSTR path) {
	CFile file;
	BOOL result;

	result = file.Open(path,CFile::modeRead | CFile::shareDenyNone);
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}

	return file.GetLength();
}


CString GetTempPath() {
	BOOL result;
	CString tempDir;

	result = ::GetTempPath(MAX_PATH,tempDir.GetBuffer(MAX_PATH)) != 0;
	tempDir.ReleaseBuffer();
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}

	return tempDir;
}

CString CreateTempFile(LPCTSTR prefix) {
	ASSERT(prefix == NULL || AfxIsValidString(prefix));
	CString tempFile;
	CString tempDir;
	BOOL    result;

	tempDir = GetTempPath();
	result = ::GetTempFileName(tempDir,prefix,0,tempFile.GetBuffer(MAX_PATH)) != 0;
	tempFile.ReleaseBuffer();
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}

	return tempFile;
}

CString CreateTempDirectory(LPCTSTR prefix) {
	CString tempDir;
	BOOL    result;

	tempDir = CreateTempFile(prefix);

	result = ::DeleteFile(tempDir);
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}

	result = ::CreateDirectory(tempDir,NULL) != 0;
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}

	return tempDir;
}

void WriteFile(LPCTSTR path,LPCTSTR string) {
	ASSERT(AfxIsValidString(path));
	ASSERT(AfxIsValidString(string));
	BOOL result;
	CFile file;

	result = file.Open(path,
		               CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}
	file.Write(string,strlen(string));
	file.Close();
}

CString ReadFile(LPCTSTR path) {
	ASSERT(AfxIsValidString(path));
	ASSERT(GetFileAttributes(path) != -1);
	CFile file;
	UINT length;
	CString text;
	BOOL result;

	result = file.Open(path,CFile::modeRead | CFile::shareDenyNone);
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}

	length = file.GetLength();
	result = file.Read(text.GetBuffer(length),length) == length;
	text.ReleaseBuffer();
	if (!result) {
		ASSERT(FALSE);
		AfxThrowFileException(CFileException::generic ,GetLastError(),NULL);
	}
	file.Close();

	return text;
}

BOOL GetFileVersion(LPCTSTR ptszPath,UINT& nMaj,UINT& nMin) {
	nMaj = 0;
	nMin = 0;

	ASSERT(ptszPath);
	ASSERT(AfxIsValidString(ptszPath));

	DWORD dummy = 0;
	DWORD dwVersionSize = GetFileVersionInfoSize(const_cast<LPTSTR>(ptszPath),&dummy);
	if (dwVersionSize == 0)
		return FALSE;

	LPBYTE buffer = (LPBYTE)_alloca(dwVersionSize);

	if (!GetFileVersionInfo(const_cast<LPTSTR>(ptszPath),0,dwVersionSize,buffer)) {
		return FALSE;
	}
	
	VS_FIXEDFILEINFO* pInfo = NULL;
	UINT uiFileVersionSize = 0;
	LPVOID pv;
	VERIFY(VerQueryValue(buffer,_T("\\"), &pv, &uiFileVersionSize));
	pInfo = reinterpret_cast<VS_FIXEDFILEINFO*>(pv);
	ASSERT(AfxIsValidAddress(pInfo,uiFileVersionSize));

	nMaj = pInfo->dwFileVersionMS;
	nMin = pInfo->dwFileVersionLS;

	return TRUE;
}

CString GetFileVersion(LPCTSTR ptsz) {
	int nV1;
	int nV2;
	int nV3;
	int nV4;
	if (GetFileVersion(ptsz,nV1,nV2,nV3,nV4)) {
		CString version;
		version.Format(_T("%03d.%03d.%03d.%03d"),nV1,nV2,nV3,nV4);
		return version;
	} else {
		return _T("-.-.-.-");
	}
}

BOOL GetFileVersion(LPCTSTR ptszPath,int& nV1,int& nV2,int& nV3,int& nV4) {
	UINT nMaj = 0;
	UINT nMin = 0;
	if (GetFileVersion(ptszPath,nMaj,nMin)) {
		nV1 = nMaj / 0x10000;
		nV2 = nMaj % 0x10000;
		nV3 = nMin / 0x10000;
		nV4 = nMin % 0x10000;

		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL ExistsDirectory(LPCTSTR path) {
	ASSERT(path);
	ASSERT(AfxIsValidString(path));

	DWORD dwResult;
	dwResult = ::GetFileAttributes(path);
	if (dwResult == 0xFFFFFFFF) {
		return FALSE;
	}

	if (dwResult & FILE_ATTRIBUTE_DIRECTORY) {
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL ExistsPath(LPCTSTR path) {
	ASSERT(path);
	ASSERT(AfxIsValidString(path));

	DWORD dwResult;
	dwResult = ::GetFileAttributes(path);
	if (dwResult == 0xFFFFFFFF) {
		return FALSE;
	}

	if (! (dwResult & FILE_ATTRIBUTE_DIRECTORY)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL OpenURL(LPCTSTR path) {
	ASSERT(AfxIsValidString(path));

	HINSTANCE h = ::ShellExecute(NULL,_T("open"),path,NULL,NULL,SW_SHOWNORMAL);
	if (32 <= UINT(h)) {
		return TRUE;
	}
	return FALSE;
}

CString GetFileName(LPCTSTR ptszPath) {
	UINT n = max(lstrlen(ptszPath),MAX_PATH);
	
	CString str;
	AfxGetFileName(ptszPath,str.GetBuffer(n),n);
	str.ReleaseBuffer();
	return str;
}

CString GetFileExt(LPCTSTR ptsz) {
	ASSERT(ptsz != NULL && AfxIsValidString(ptsz));

	CString strExt;
	CString str = ptsz;
	int n = str.ReverseFind('.');
	if (n == -1)
		strExt.Empty();
	else
		strExt = str.Mid(n+1);

	return strExt;
}

CString GetFileStripedName(LPCTSTR ptsz) {
	ASSERT(ptsz != NULL && AfxIsValidString(ptsz));

	CString str = GetFileName(ptsz);
	int nDot = str.ReverseFind(_T('.'));
	if (nDot != -1)
		str = str.Left(nDot);

	return str;
}

#if defined(_DEBUG)

class FileUtilTest : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(FileUtilTest);
  CPPUNIT_TEST(testGetCurrentDirectory);
  CPPUNIT_TEST_SUITE_END();
private:

public:
	FileUtilTest() {}
	virtual ~FileUtilTest() {}
	virtual void setUp() {}
	virtual void tearDown() {}
	
	void testGetCurrentDirectory() {
		CString saved = GetCurrentDirectory();
		
		SetCurrentDirectory("c:\\");
		CPPUNIT_ASSERT(GetCurrentDirectory() == "c:\\");

		SetCurrentDirectory(saved);

		CPPUNIT_ASSERT(GetCurrentDirectory() == saved);
	}

	void testTemp() {
		const int THREADS = 16;
		int   n;
		CWinThread* threads[THREADS];

		failed = FALSE;

		for (n = 0;n < THREADS;n++) {
			threads[n] = ::AfxBeginThread(runTestTemp,0,0,0,0,NULL);
		}
	
		for (n = 0;n < THREADS;n++) {
			::WaitForSingleObject(*threads[n],INFINITE);
		}

		CPPUNIT_ASSERT(!failed);
	}

	static BOOL failed;
	static UINT runTestTemp(LPVOID pv) {
		try {
			for (int n = 0;n < 256;n++) {
				CreateTempDirectory("");
				CreateTempFile("");
			}
		} catch(CException* e) {
			TCHAR error[256];
			VERIFY(e->GetErrorMessage(error,256,NULL));
			TRACE("%s\n",error);
			failed = TRUE;
		}
		return 0;
	}

	void testReadWrite() {
		CString path = CreateTempFile("");

		CString text = "•\”\‰F’ˆA\\\t";
		WriteFile(path,text);
		CPPUNIT_ASSERT(GetFileLength(path) == 11);

		CString read;
		read = ReadFile(path);

		CPPUNIT_ASSERT(read == text);
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(FileUtilTest);

#endif
