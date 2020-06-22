#pragma once

CString GetCurrentDirectory(); 
CString GetAppExePath();
CString GetAppRootPath();
CString GetAppFilePath(LPCTSTR ptszRPath);
CString CatPath(LPCTSTR,LPCTSTR);
CString GetFileParent(LPCTSTR);
CString GetFileName(LPCTSTR ptszPath);
CString GetFileStripedName(LPCTSTR ptsz);
CString GetFileExt(LPCTSTR ptsz);
UINT GetFileLength(LPCTSTR);
CString CreateTempDirectory(LPCTSTR);
CString CreateTempFile(LPCTSTR);
CString GetTempPath();
CString ReadFile(LPCTSTR);
void WriteFile(LPCTSTR,LPCTSTR);
BOOL GetFileVersion(LPCTSTR path,UINT& major,UINT& minor);
BOOL GetFileVersion(LPCTSTR,int&,int&,int&,int&);
CString GetFileVersion(LPCTSTR path);
BOOL OpenURL(LPCTSTR path);
BOOL ExistsDirectory(LPCTSTR path);
BOOL ExistsPath(LPCTSTR path);

