#pragma once

/**
	CPaintDC��CPaintMemDC�𓝍������N���X
	WINDOWS�`��ƃo�b�N�T�[�t�F�[�X(������)�`�擝��
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
