#pragma once

class BitmapButtonEx : public CButton {
private:
	enum {NormalState = 0,DownState = 1,DisableState = 2,STATE_COUNT = 3};
	CBitmap m_bmp;
	int m_nWidth;	//	�{�^����
	int m_nHeight;	//	�{�^����
	int m_nSrcX;	//	�{�^���摜�J�n�ʒu X
	int m_nSrcY;	//	�{�^���摜�J�n�ʒu Y

public:
	BitmapButtonEx();
	BOOL LoadBitmap(UINT n,int nWidth = -1);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~BitmapButtonEx();

protected:
	afx_msg UINT OnGetDlgCode();
	DECLARE_MESSAGE_MAP()
};

