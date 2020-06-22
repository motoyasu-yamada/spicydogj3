#pragma once

#include "view/CharView.h"
#include "model/char/TextChar.H"
#include "utility/wnd/PreviewStatic.H"
#include "utility/wnd/ColorBox.h"

class TextCharView
:	public CharDialog,
	public PreviewStatic::PaintEventListener {
private:
	BOOL m_inited;
	BOOL m_bReenterLock;
	ColorBoxEx      m_colorbox;
	CComboBox	    m_cbFontSize;
	CComboBox	    m_cbFontFace;
	PreviewStatic	m_psPreview;
	CEdit	        m_editText;
	CSpinButtonCtrl	m_spinH;
	CSpinButtonCtrl	m_spinW;
	TextCharState   m_state;

protected:
	DECLARE_DYNCREATE(TextCharView)
	TextCharView();

private:
	virtual ~TextCharView();
	virtual void OnPSPaint(PreviewStatic&);
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	SpicydogDoc* GetDocument();
	TextChar* GetTextChar();
	void UpdateControls();
	void EnableCustomFontProps(BOOL);
	void SetButtonIcon(UINT nID,UINT nRes);
	afx_msg LRESULT OnTextColor(WPARAM,LPARAM);;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelFontType();
	afx_msg void OnUpdateData();
	DECLARE_MESSAGE_MAP()
};
