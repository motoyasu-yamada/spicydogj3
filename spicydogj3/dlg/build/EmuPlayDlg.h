#pragma once
#include "utility/CmdExec.h"

class Builder;

class EmuPlayDlg : public CDialog,public CCmdExecListener {
private:
	CBrush   m_outputBkgnd;
	CEdit	 m_editOutput;
	CString  m_temp;
	Builder* m_builder;
	CCmdExec m_exec;
public:
	EmuPlayDlg(LPCTSTR jadPath,Builder* builder,BOOL imediate);
protected:
	BOOL IsRunning() const;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void Run();
	void UpdateCtrlEx(BOOL);
	afx_msg void OnChanged();
	afx_msg void OnEmuTerm();
	afx_msg void OnBrowse();
	afx_msg void OnDetect();
	afx_msg void OnClear();
	afx_msg void OnRun();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOutput(LPCTSTR);
	virtual void OnTerm(BOOL);
	DECLARE_MESSAGE_MAP()
};
