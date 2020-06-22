#pragma once

class ScrollControler {
private:
	CWnd* m_pParent;
	UINT m_nSBCode;
	UINT m_nPos;
	CScrollBar* m_pScrollBar;
	int  m_nMax;
	int  m_nPage;
	int& m_nNext;
public:
	ScrollControler(CWnd* pParent,int&,UINT nSBCode, UINT nPos, CScrollBar* pScrollBar,int nMax,int nPage);
	void Scroll();
};