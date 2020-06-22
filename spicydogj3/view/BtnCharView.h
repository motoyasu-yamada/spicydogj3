#pragma once

#include "dlg/action/ActionListDlg.H"
#include "model/char/BtnChar.h"
#include "utility/wnd/PreviewStatic.H"
#include "view/CharView.h"
#include <vector>

class SpicydogDoc;

/** ボタンキャラクタ編集画面 */
class BtnCharView :
	public CharDialog,
	public PreviewStatic::PaintEventListener {
private:
	typedef std::vector<const Char*> BTNABLE_CHAR_LIST;
	static CImageList s_images;
	static CString    s_strUnsetItem;
	BTNABLE_CHAR_LIST  m_chars;
	BtnCharState       m_state;
	ActionListDlg      m_dlgActionList;
	CComboBoxEx	       m_cbNormal;
	CComboBoxEx	       m_cbActive;
	PreviewStatic	   m_psPreviewN;
	PreviewStatic	   m_psPreviewA;
	BOOL               m_inited;

public:
	DECLARE_DYNCREATE(BtnCharView)
	BtnCharView();
	static void clinit();

private:
	SpicydogDoc* GetDocument();
	BtnChar* GetBtnChar();
	void SetButtonIcon(UINT nID,UINT nRes);
	void UpdateControls();
	void UpdateCharList();
	void InitBtnableCharList();
	afx_msg void OnUpdateData();
	afx_msg void OnEditMailEvent();
	afx_msg void OnEditTelEvent();
	afx_msg void OnEditScheduleEvent();
	virtual void OnPSPaint(PreviewStatic&);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	
	DECLARE_MESSAGE_MAP()
};