#pragma once

class CursorClipper {
public:
	CursorClipper(LPCRECT prcClip);
	~CursorClipper();
private:
	CRect m_rcOld;
};

class CursorCapture {
private:
	HWND m_hwndOld;
	BOOL m_bCapturing;	
public:
	CursorCapture(HWND = NULL);
	~CursorCapture();
	BOOL IsCapturing() const;
	void SetCapture(HWND hwnd);
	void ReleaseCapture();
};
