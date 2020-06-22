#pragma once

class SplashWnd : public CWnd {
private:
	static SplashWnd* s_pSingleInstance;
	CBitmap	m_bmpSplash;
	CSize m_szBitmap;

public:
	static void DoSplash(UINT resid);
	static void EndSplash(UINT = 0);
private:
	SplashWnd(UINT resid);
	virtual ~SplashWnd();
	virtual void PostNcDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void _DoSplash();
	void _EndSplash(UINT = 0);
	void HideSplash(UINT);

	DECLARE_MESSAGE_MAP()
};