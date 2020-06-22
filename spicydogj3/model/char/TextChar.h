#pragma once

#include "Char.h"

/** テキスト矩形の最大高 */
const int MAX_TEXT_HEIGHT = 1000;
/** テキスト矩形の最大幅 */
const int MAX_TEXT_WIDTH  = 1000;

/** フォントフェース
	下記の値は0からの連番であること。
	コンボボックス等のインデックスから自動的に下記値に変換する為に */
enum {
	/** Font.FACE_SYSTEM(0) */
	TC_FACE_SYSTEM       = 0,
	/** Font.FACE_MONOSPACE(32) */
	TC_FACE_MONOSPACE    = 1,
	/** Font.FACE_PROPORTIONAL(64) */
	TC_FACE_PROPORTIONAL = 2
};

/** フォントサイズ 
	下記の値は0からの連番であること。
	コンボボックス等のインデックスから自動的に下記値に変換する為に */
enum {
	/** Font.SIZE_SMALL(8) */
	TC_SIZE_SMALL  = 0,
	/** Font.SIZE_MEDIUM(0) */	
	TC_SIZE_MEDIUM = 1,
	/** Font.SIZE_LARGE(16) */
	TC_SIZE_LARGE  = 2
};

/** テキストキャラクタステート */
class TextCharState {
public:
	CString  text;
	BOOL     autoResize;
	COLORREF textColor;
	int width;
	int height;
	int fontFace;
	int fontSize;
	BOOL fontStyleBold;
	BOOL fontStyleItalic;
	BOOL fontStyleUnderline;

	TextCharState();
	TextCharState& operator= (const TextCharState&);
};

class SDOMArchive;
class Builder;

/** ネイティブフォント/オリジナルフォントを利用したテキストキャラクタ */
class TextChar : public Char {
	friend class TextCharTest;
	friend class TextCharView;
private:
	static CharType TEXT_CHAR_TYPE;
	TextCharState m_state;
public:
	TextChar();
	TextChar(const TextChar&);
	TextChar(LPCTSTR);
	void SetState(const TextCharState& state);
	void GetState(TextCharState& state);
	static CString CreateNameByBody(LPCTSTR body);
private:
	int GetFontStyle() const;
	virtual ~TextChar();
	virtual Char* Clone() const;
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual const CharType& GetCharType() const;
	virtual void DrawPreview(const CharPreview*) const;
	virtual void GetRect(CRect&) const;
	virtual void Draw(CDC* pDC,const AffineMatrix*,const Coloring*) const;
	static void UpdateAutoSize(TextCharState*);
	static void DrawPreview(const TextCharState*,CDC* pDC,CRect rc,COLORREF crBkgnd);
};
