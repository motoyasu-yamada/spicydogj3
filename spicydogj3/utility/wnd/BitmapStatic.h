#pragma once

/**
	BitmapStatic
	・マウスオーバーによるエッチング
	・クリックによる凹み
	・無効化処理
	・クリック時にコマンドスロー
*/
class BitmapStatic : public CStatic {
private:
	CImageList m_imagelist;
	UINT m_nCurID;
	BOOL m_bHover;
	BOOL m_bDown;

public:
	BitmapStatic();
	void Setup(UINT nCurID,UINT nBmpID);
	BOOL Create(UINT nCurID,UINT nBmpID,CWnd* pParent,UINT nID);
	virtual ~BitmapStatic();
protected:
	BOOL HitTest(CPoint pt) const;
	virtual void PreSubclassWindow();
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

