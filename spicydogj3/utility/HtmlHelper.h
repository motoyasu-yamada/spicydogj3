#pragma once

class HtmlHelper {
private:
	BOOL m_inited;
	BOOL m_uninitialized;
	DWORD m_dwCookie;
	HWND m_hWnd;
	CString m_strFilePath;
	CString m_strWndType;
	
public:
	HtmlHelper();
	
	void DisplayToc(LPCTSTR ptszPage);
	void DisplayIndex(LPCTSTR ptszKeyword);
	void DisplaySearch(LPCTSTR ptszQuery);
	void CloseAll();

	void Initialize(HWND hwnd,LPCTSTR ptszPath,LPCTSTR ptszWndType);
	void Uninitialize();
	BOOL PreTranslateMessage(MSG* pMsg);

public:
	~HtmlHelper();
};

