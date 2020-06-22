#pragma once

#include "Char.h"

/** �e�L�X�g��`�̍ő卂 */
const int MAX_TEXT_HEIGHT = 1000;
/** �e�L�X�g��`�̍ő啝 */
const int MAX_TEXT_WIDTH  = 1000;

/** �t�H���g�t�F�[�X
	���L�̒l��0����̘A�Ԃł��邱�ƁB
	�R���{�{�b�N�X���̃C���f�b�N�X���玩���I�ɉ��L�l�ɕϊ�����ׂ� */
enum {
	/** Font.FACE_SYSTEM(0) */
	TC_FACE_SYSTEM       = 0,
	/** Font.FACE_MONOSPACE(32) */
	TC_FACE_MONOSPACE    = 1,
	/** Font.FACE_PROPORTIONAL(64) */
	TC_FACE_PROPORTIONAL = 2
};

/** �t�H���g�T�C�Y 
	���L�̒l��0����̘A�Ԃł��邱�ƁB
	�R���{�{�b�N�X���̃C���f�b�N�X���玩���I�ɉ��L�l�ɕϊ�����ׂ� */
enum {
	/** Font.SIZE_SMALL(8) */
	TC_SIZE_SMALL  = 0,
	/** Font.SIZE_MEDIUM(0) */	
	TC_SIZE_MEDIUM = 1,
	/** Font.SIZE_LARGE(16) */
	TC_SIZE_LARGE  = 2
};

/** �e�L�X�g�L�����N�^�X�e�[�g */
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

/** �l�C�e�B�u�t�H���g/�I���W�i���t�H���g�𗘗p�����e�L�X�g�L�����N�^ */
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
