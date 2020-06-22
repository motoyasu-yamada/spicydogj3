#pragma once

class UploadingDlg : public CDialog {
public:
	UploadingDlg();
	void Create();
	void SetProgress(UINT);
private:
	DECLARE_MESSAGE_MAP()
};

