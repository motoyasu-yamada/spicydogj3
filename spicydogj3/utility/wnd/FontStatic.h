#pragma once

class FontStatic : public CStatic {
private:
	typedef std::auto_ptr<CFont> FONTPTR;
	FONTPTR m_pFont;
public:
	FontStatic();
	virtual ~FontStatic();
	void GetLogFont(LOGFONT&);
	void SetLogFont(const LOGFONT&);
	void AdjustSize();
private:
	DECLARE_MESSAGE_MAP()
};