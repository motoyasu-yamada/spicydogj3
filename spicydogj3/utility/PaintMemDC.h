#pragma once

/**
	CPaintDCとCPaintMemDCを統合したクラス
	WINDOWS描画とバックサーフェース(メモリ)描画統合
*/
class PaintMemDC : public CDC {
private:
	BOOL m_bFlipped;
	BOOL m_bMemed;
	CRect m_rc;
	CPaintDC m_dcPaint;
	CBitmap  m_bmpMem;
	CBitmap* m_pbmpSaved;
public:
	PaintMemDC(CWnd*);
	virtual void SetViewportOrg(CPoint);
	virtual ~PaintMemDC();
	void Flip();
};
