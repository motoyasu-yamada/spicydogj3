#pragma once

/**	ColorFaceButton ウィンドウ */
class ColorFaceButton : public CButton {
private:
	COLORREF m_cr;
public:
	ColorFaceButton();
	virtual ~ColorFaceButton();
	COLORREF GetColor() const;
	void SetColor(COLORREF);
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClicked();

	DECLARE_MESSAGE_MAP()
};
