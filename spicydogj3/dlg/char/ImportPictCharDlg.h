#pragma once

#include <AfxTempl.h>

class ImportPictCharDlg : public CFileDialog {
private:
	CArray<TCHAR,TCHAR&> m_buffer;
public:
	LPTSTR m_paths;
	BOOL   m_pathThrough;
	ImportPictCharDlg(CWnd*);
protected:
	BOOL OnFileNameOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};
