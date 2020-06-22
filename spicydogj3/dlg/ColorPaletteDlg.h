#pragma once

#include "utility/wnd/PreviewStatic.H"
#include "utility/wnd/ColorBox.h"
#include "utility/wnd/BitmapButtonEx.h"
#include "model/type/Act.h"

class ColorPaletteDlg
:	public CDialog,
	public PreviewStatic::PaintEventListener,
	public ColorPickerListener
{
private:
	static ACTTYPE s_nDefaultAct;
	BitmapButtonEx	m_btnClose;
	BitmapButtonEx	m_btnSpoit;
	BitmapButtonEx	m_btnNull;
	BitmapButtonEx	m_btnDlg;
	CEdit	     	m_editClrVal;
	PreviewStatic	m_ColorPal;
	ColorBox		m_ColorBox;
	CComboBox	    m_cbACTs;
	COLORREF m_crCurrent;
	COLORREF m_crSelect;
	BOOL    m_bEnableTrans;
	int		m_nCellW;
	int		m_nCellH;
	int		m_nPalW;
	int		m_nPalH;
	int		m_xInit;
	int		m_yInit;

public:
	ColorPaletteDlg(COLORREF cr,BOOL bTrans,int x,int y,CWnd* pParent = NULL);
	COLORREF GetColor() const;

private:
	const Act* GetCurAct() const;
	void UpdateACTsComboBox();
	void SetCurrentColor(COLORREF cr);
	void SelectColor(COLORREF cr);
	void Cancel();
	void SetupButtons();
	void AdjustPalletteBox();
	COLORREF  GetColorFromPoint(const CPoint&) const;
	void OnMouseDown(CPoint);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnPickerMove(CPoint,COLORREF);
	virtual void OnPSPaint(PreviewStatic&);
	virtual BOOL OnPSEraseBkgnd(PreviewStatic&,CDC&);
	virtual void OnPSMouseMove(PreviewStatic&,UINT,CPoint);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClrVal();
	afx_msg void OnClrDlg();
	afx_msg void OnClrNull();
	afx_msg void OnClrSpoit();
	afx_msg void OnUpdateClrval();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPalette();
	afx_msg void OnClrClose();
	afx_msg void OnSelchangeAct();
	DECLARE_MESSAGE_MAP()
};
