#pragma once

class BitmapButtonEx : public CButton {
private:
	enum {NormalState = 0,DownState = 1,DisableState = 2,STATE_COUNT = 3};
	CBitmap m_bmp;
	int m_nWidth;	//	ボタン幅
	int m_nHeight;	//	ボタン高
	int m_nSrcX;	//	ボタン画像開始位置 X
	int m_nSrcY;	//	ボタン画像開始位置 Y

public:
	BitmapButtonEx();
	BOOL LoadBitmap(UINT n,int nWidth = -1);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~BitmapButtonEx();

protected:
	afx_msg UINT OnGetDlgCode();
	DECLARE_MESSAGE_MAP()
};

