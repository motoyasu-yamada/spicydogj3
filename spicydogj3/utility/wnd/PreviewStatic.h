#pragma once

class PreviewStatic : public CStatic {
	public:
	class PaintEventListener
	{
	public:
		virtual void OnPSPaint(PreviewStatic&);
		virtual BOOL OnPSEraseBkgnd(PreviewStatic&,CDC&);
		virtual void OnPSNcPaint(PreviewStatic&);
		virtual void OnPSMouseMove(PreviewStatic&,UINT,CPoint);
	};
	friend class PaintEventListener;

private:
	PaintEventListener* m_pListener;

public:
	PreviewStatic();
	void SetListener(PaintEventListener*);
	virtual ~PreviewStatic();
private:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
