#pragma once

#include "utility/DocumentEvent.h"

/**	CView�ɑ������鏈��������N���X */
class DialogBarDlg : public CDialog,public DocumentListener {
private:
	BOOL m_bListening;
	CDocument* m_pActiveDocument;

public:
	DECLARE_DYNAMIC(DialogBarDlg)
	DialogBarDlg();
	virtual ~DialogBarDlg();

protected:
	CDocument* GetDocument();
	BOOL ExistActiveDocument() const;
	virtual void OnDocumentActivated(CDocument* p);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
};
