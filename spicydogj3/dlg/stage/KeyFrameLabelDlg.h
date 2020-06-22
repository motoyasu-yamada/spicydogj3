#pragma once

#include "model/stage/KeyFrame.h"
class Appli;
class KeyFrameLabelDlg : public CDialog {
	const Appli*  m_appli;
	const KeyFrameLabel* m_labelOrg;
public:
	KeyFrameLabel editLabel;
	KeyFrameLabelDlg(const Appli*,const KeyFrameLabel*,CWnd* parent);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};