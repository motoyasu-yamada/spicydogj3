#pragma once

class HyperlinkLabel : public CStatic {
private:
	typedef void (*PFUNC_OPEN_URL)(LPCTSTR ptszUrl);
private:
	enum {ID_TOOLTIP=0};

	TRACKMOUSEEVENT m_tme;
	BOOL		m_bMouseOver;
	BOOL		m_bMouseDown;

	COLORREF	m_crFore;
	COLORREF	m_crOver;
	COLORREF	m_crDown;
	HCURSOR		m_hcurOver;
	CString		m_strCaption;
	CString		m_strUrl;

	PFUNC_OPEN_URL m_pfuncOpenUrl;

	CFont		m_font;
	CToolTipCtrl m_ToolTip;

public:
	HyperlinkLabel();
	
	void SetColors(COLORREF,COLORREF,COLORREF);
	void SetCursor(HCURSOR);
	void SetUrl(LPCTSTR);
	void SetCaption(LPCTSTR);
	PFUNC_OPEN_URL SetUrlCallback(PFUNC_OPEN_URL pfunc);

public:
	virtual ~HyperlinkLabel();
	virtual void OpenUrl(LPCTSTR ptszUrl);
private:
	void AdjustSize();
	void EnableClick();
	void SetUnderlineFont();
	void EnableUrlToolTips();
	
	static void InternalOpenUrl(LPCTSTR ptszUrl);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	afx_msg void	OnClicked();
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor(CWnd*,UINT,UINT);
	afx_msg HBRUSH	CtlColor(CDC* pDC, UINT nCtlColor); 
	afx_msg LRESULT OnMouseLeave(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
};