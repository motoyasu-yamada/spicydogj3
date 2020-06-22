#pragma once

#include "Char.H"
#include "model/type/Act.h"
#include "sdom/SDOMEnum.h"
#include <ltwrappr.h>

typedef BOOL (CALLBACK *PICT_EFFECT_PROC)(BITMAPHANDLE*,int);

/** �摜�̃T�C�Y�Ƃ��ċ������c�E���T�C�Y */
const int PICT_MIN_WIDTH  = 1;
const int PICT_MAX_WIDTH  = 1024;
const int PICT_MIN_HEIGHT = 1;
const int PICT_MAX_HEIGHT = 1024;
/** �摜�̃f�[�^�T�C�Y�Ƃ��ċ������e�� */
const int PICT_DATA_MIN_LENGTH = 1;
const int PICT_DATA_MAX_LENGTH = 0xFFFF;

/** �摜�f�[�^ */
class PictData {
	friend class PictChar;
public:
	/** DIB�r�b�g�}�b�v�摜 */
	LBitmapBase m_bmp;
	/** �p�X�X���[�f�[�^�摜 */
	LBuffer     m_data;
	/** �f�t�H���g��� */
	PictData();
	/** ���� */
	PictData(const PictData&);
	PictData& operator = (const PictData&);

private:
	/** ���K���`�F�b�N */
	void AssertValid() const;
	/** �����w���p�[ */
	void CopyFrom(const PictData&);
};


/** �摜�œK������ */
enum PICT_QUANTIZE {
	PICT_QUANTIZE_PATHTHROUGH = 0,
	PICT_QUANTIZE_PNG         = 1,
	PICT_QUANTIZE_JPG         = 2
};
ENUM_TYPE(PictQuantizeType);
ENUM_TYPE_ITEM(PictQuantizeType,PICT_QUANTIZE_PATHTHROUGH,ASDOMN_PATH_THROUGH);
ENUM_TYPE_ITEM(PictQuantizeType,PICT_QUANTIZE_PNG,ASDOMN_QUANTIZE_PNG);
ENUM_TYPE_ITEM(PictQuantizeType,PICT_QUANTIZE_JPG,ASDOMN_QUANTIZE_JPG);


ENUM_TYPE(PaletteType);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_IMODE, ASDOMV_IMODE_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_WEB,   ASDOMV_WEB_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_WIN,   ASDOMV_WIN_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_MAC,   ASDOMV_MAC_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_GRAY,  ASDOMV_GRAY_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_MONO,  ASDOMV_MONO_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_RGB,   ASDOMV_RGB_PALETTE);
ENUM_TYPE_ITEM(PaletteType,ACTTYPE_CUSTOM,ASDOMV_CUSTOM_PALETTE);

/** �摜�œK���p�����[�^ */
class PictQuantizeParam {
public:
	int      m_quantize;
	/** JPEG���k�� 0%-100% */
	int      m_jpgCompress;
	/** �p���b�g�`�� */
	int      m_pngPaletteType;
	/** �p���b�g���f�[�^ */
	Act      m_pngPalette;
	/** �J���[�� */
	int      m_pngColors;
	/** �f�B�U���������邩�ǂ����H */
	BOOL     m_pngUseDither;
	/** �����F�𗘗p���邩�ǂ����H */
	BOOL     m_pngUseTransparency;
	/** �����F */
	COLORREF m_pngTransparentColor;

	PictQuantizeParam();
	PictQuantizeParam(const PictQuantizeParam&);
	PictQuantizeParam& operator = (const PictQuantizeParam&);
private:
	friend class PictChar;
	void Init();
	void AssertValid() const;
	void CopyFrom(const PictQuantizeParam&);
};


/** �摜�œK���L���b�V�� */
enum PICTOPTOUTFORMAT {
	PICTOPTOUTFORMAT_PNG = 0,
	PICTOPTOUTFORMAT_JPG = 1,
};
class PictOptCache {
public:
	BOOL             m_bValid;
	LBitmapBase      m_bmp;
	LBuffer          m_data;
	PICTOPTOUTFORMAT m_nFormat;
	int				 m_nDataLen;
	BOOL             m_bTrans;
	COLORREF         m_crTrans;
	CSize			 m_sz;
public:
	PictOptCache();
	/** �s�N�`���̍œK�� */
	BOOL Optimize(const PictData&,const PictQuantizeParam&);
private:
	friend class PictChar;
	void Init();
	void AssertValid() const;
}; // struct PictOptCache


/**	�s�N�`���L�����N�^ */
class PictChar : public Char {
	friend class PictModDataCmd;
	friend class PictSetOptParamCmd;
	friend class PictSetCenterCmd;
	friend class PictCharView;
private:
	/** �s�N�`���L�����N�^�^���*/
	static CharType PICT_CHAR_TYPE;
	/** �摜�̃A�j���[�V�������S�_ */
	CPoint               m_centerPoint;
	/** �摜�̃f�[�^     */
	PictData             m_data;
	/** �摜�̍œK���ݒ� */
	PictQuantizeParam    m_quantize;
	/** �摜�̕\���L���b�V�� */
	mutable PictOptCache m_cache;
public:
	PictChar();
	/**	(�J�X�^���R���X�g���N�^)
		��̃s�N�`���L�����N�^�𐶐�����
		
		@param		sz �V�K�L�����N�^�̃T�C�Y
					cx/cy 1 pixel �` 65535 pixel �ȓ�
		@param		cr �h��Ԃ��F
		@return		NULL - �������s���ȏꍇ
		@exception	CMemoryException ���������A���P�[�g�ł��Ȃ������ꍇ */
	static PictChar* newEmpty(CSize sz,COLORREF cr);
	/**	(�J�X�^���R���X�g���N�^)
		�p�X�X���[���o�ݒ�^�̃s�N�`�����C���|�[�g����
		
		@param		�p�X�X���[�s�N�`�������݂���p�X
		@return		NULL - 	�t�@�C�������݂��Ȃ��ꍇ
							���Ή��̃t�@�C���̏ꍇ
		@exception	CMemoryException ���������A���P�[�g�ł��Ȃ������ꍇ */
	static PictChar* newImportPathThrough(LPCTSTR ptszPath);
	/**	(�J�X�^���R���X�g���N�^)
		�p�X�X���[���o�ݒ�^�̃s�N�`�����C���|�[�g����
		
		@param		�p�X�X���[�s�N�`�������݂���p�X
		@return		NULL - 	�t�@�C�������݂��Ȃ��ꍇ
							���Ή��̃t�@�C���̏ꍇ
							�t�@�C���T�C�Y���K����傫�������ꍇ
		@exception	CMemoryException ���������A���P�[�g�ł��Ȃ������ꍇ */
	static PictChar* newImport(LPCTSTR ptszPath);
	/**	(�J�X�^���R���X�g���N�^)
		�N���b�v�{�[�h�̉摜�������Ƀs�N�`���L�����N�^�𐶐�����
		
		@param hbmp	Win �r�b�g�}�b�v�n���h��
		@param hpal	Win �p���b�g�n���h��
		@return		NULL - 	���Ή��̃t�@�C���̏ꍇ
							�t�@�C���T�C�Y���K����傫�������ꍇ
		@exception	CMemoryException ���������A���P�[�g�ł��Ȃ������ꍇ */
	static PictChar* newPaste(HBITMAP,HPALETTE);

	virtual Char* Clone() const;

	virtual void Serialize(SDOMArchive*);
	virtual void Build(Builder*) const;
	virtual void AssertValid() const;


	/// �œK���ݒ肨��уr�b�g�}�b�v���ҏW�\���ǂ���
	BOOL IsEditable() const;
	///  �ҏW�p�̖��œK���`��
	void DrawEditview(CDC* pDC) const;
	/// �摜�̃Z���^�����O�ݒ���擾
	CPoint GetCenter() const;
	/// �摜�̏��o���ݒ���擾
	const PictQuantizeParam& GetPictOptParam() const;
	/// �摜�̃r�b�g�}�b�v�f�[�^�̎擾
	const PictData& GetPictData() const;
	/// �œK���ω摜�̃G�N�X�|�[�g
	BOOL ExportCache(LPCTSTR) const;

	static void CreateAct(Act& act,const LBitmapBase& bmp,const PictQuantizeParam& param);
	static BOOL LoadBmp  (LPCTSTR,PictData&);
	static BOOL Optimize (PictOptCache&,const PictData&,const PictQuantizeParam&);

private:
	static PictChar* newPictChar(const PictData&,const PictQuantizeParam&);
	PictChar(const PictChar& rhs);
	virtual const CharType& GetCharType() const;
	virtual void DrawPreview(const CharPreview*) const;
	virtual void Draw(CDC* pDC,const AffineMatrix*,const Coloring*) const;
	virtual void GetRect(CRect& rc) const;
	void SetCenter(CPoint);
	void SetPictData(const PictData&);
	void SetPictOptParam(const PictQuantizeParam&);
	CSize  GetBitmapSize() const;
	BOOL Optimize() const;
	int GetColorBits() const;
	static BOOL Data2Bmp(const LBuffer&,LBitmapBase&);
	static BOOL EditByExe(PictData&);
	virtual CString GetInfo() const;
};





