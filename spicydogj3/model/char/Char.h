#pragma once

#include "model/build/Buildable.h"
#include "sdom/olm.h"
#include "utility/Bezier.h"

class COleDataSource;
class SDOMArchive;
class AffineMatrix;
class Coloring;

/** �v���r���[�`���� */
class CharPreview {
public:
	CDC*     dc;
	CRect    rect;
	COLORREF bkgndColor;
};

/** �L�����N�^�^��� */
class CharType {
public:
	CharType(
		CRuntimeClass* guiClass,
		LPCTSTR charTypeName,
		UINT    displayName,
		LPCWSTR xmlNodeName,
		int  iconIndex,
		BOOL guiModal,
		BOOL canButtonChar,
		BOOL canSoundChar,
		BOOL canAttachLayer,
		BOOL canModifyColoring,
		BOOL canModifyPlacing
	);
	CRuntimeClass* guiClass;
	CString charTypeName;
	UINT    displayName;
	LPCWSTR xmlNodeName;
	int  iconIndex;
	BOOL guiModal;
	BOOL canButtonChar;
	BOOL canSoundChar;
	BOOL canAttachLayer;
	BOOL canModifyColoring;
	BOOL canModifyPlacing;
};

/** �L�����N�^ */
class Char 
:	public COLMObject,
	public Buildable {
private:
	CString m_name;			

protected:
	/** �R���X�g���N�^ */
	Char();
	/** �����R���X�g���N�^ */
	Char(const Char&);
public:
	virtual ~Char();
	/** �L�����N�^���̎擾 */
	CString GetName() const;
	/** �L�����N�^���̐ݒ� */
	void SetName(LPCTSTR ptsz);
	/** �������� */
	virtual Char* Clone() const = 0;
	/** �V���A���C�Y */
	virtual void Serialize(SDOMArchive*) = 0;
	/** �r���h���� */
	virtual void Build(Builder*) const = 0;
	/** �L�����N�^�^��� */
	virtual const CharType& GetCharType() const = 0;
	/** �v���r���[�`�� */
	virtual void DrawPreview(const CharPreview*) const = 0; 
	/** �`�� */
	virtual void Draw(CDC* pDC,const AffineMatrix*,const Coloring*) const = 0;
	/** �L�����N�^�^������L�����N�^�C���X�^���X�𐶐����� */
	static SDOMSerializable* factory(LPCWSTR);
	/** �L�����N�^�C���X�^���X����L�����N�^�^�����擾 */
	static LPCWSTR namer(SDOMSerializable*);
	/** �O�ڋ�`���擾���� - �K���I�[�o�[���C�h���邱�� */
	virtual void GetRect(CRect&) const = 0;
	/**	�L�����N�^�ɊO�ڂ���x�W�F�Ȑ����擾
		�I�[�o�[���C�h���Ȃ��ꍇ�͎����I��GetRect����v�Z����� */
	virtual void GetBezier(CBEZIER&) const;
	virtual CString GetInfo() const;
	virtual CString GetLongInfo() const;
};





