#pragma once
//////////////////////////////////////////////////////////////////////////
//	�����[SpicyDog]�̃\�[�X�R�[�h�ł��B
//	���쌠 �X�p�C�V�[�\�t�g�������
//	���̃\�[�X�R�[�h�Ɋւ��邷�ׂĂ̌����̓X�p�C�V�[�\�t�g���ۗL���܂��B
//
//	This is a part of SpicyDog  2001.
//	Copyright (C) 2000 Spicysoft Corporation.
//	All rights reserved.
//////////////////////////////////////////////////////////////////////////

#include "char.H"
#include "model/action/actionlist.h"

/** �L�[�C�x���g��M�}�b�v */
class KeyEvent : public SDOMSerializable {
public:
	BOOL KEY_0;
	BOOL KEY_1;
	BOOL KEY_2;
	BOOL KEY_3;
	BOOL KEY_4;
	BOOL KEY_5;
	BOOL KEY_6;
	BOOL KEY_7;
	BOOL KEY_8;
	BOOL KEY_9;
	BOOL KEY_ASTER;
	BOOL KEY_POUND;
	BOOL KEY_LEFT;
	BOOL KEY_UP;
	BOOL KEY_RIGHT;
	BOOL KEY_DOWN ;
	BOOL KEY_SELECT;
	BOOL KEY_SOFT1;
	BOOL KEY_SOFT2;

	virtual void Serialize(SDOMArchive*);
};

/** Java[TM]���[����M�C�x���g */
class JphoneMailEvent : public SDOMSerializable {
public:
	BOOL useEvent;

	BOOL standardMail;
	BOOL greetingMail;
	BOOL relayMail;
	BOOL autoMail;
	BOOL emergencyMail;
	BOOL superMail;
	BOOL skyMail;

	virtual void Serialize(SDOMArchive*);
};

/** Java[TM]�A�v�� �X�P�W���[����M�C�x���g */
class JphoneScheduleEvent : public SDOMSerializable {
public:
	BOOL useEvent;

	BOOL receiveAll;
	CString criteria;

	virtual void Serialize(SDOMArchive*);
	static BOOL IsValidCriteria(LPCTSTR);
};

/** Java[TM]�A�v�� �d�b��M�C�x���g */
class JphoneTelEvent : public SDOMSerializable {
public:
	BOOL useEvent;

	BOOL receiveAll;
	CString criteria;

	virtual void Serialize(SDOMArchive*);
	static BOOL IsValidCriteria(LPCTSTR);
};

/** BtnCharState �{�^���L�����N�^�̃v���p�e�B��� */
class BtnCharState {
public:
	BtnCharState();
	BtnCharState(const BtnCharState&);
	BtnCharState& operator=(const BtnCharState&);

	TOLMReferer<Char>   normalState;
	TOLMReferer<Char>   activeState;
	KeyEvent            keyEvent;
	JphoneMailEvent     jphoneMailEvent;
	JphoneScheduleEvent jphoneScheduleEvent;
	JphoneTelEvent      jphoneTelEvent;
private:
	void CopyFrom(const BtnCharState*);
};


/* BtnChar �{�^���L�����N�^ */
class BtnChar : public Char {
#if defined(_DEBUG)
	friend class BtnCharTest;
#endif
private:
	static CharType BTN_CHAR_TYPE;
	BtnCharState m_state;
	
public:
	ActionList actions;
	BtnChar();
	void GetState(BtnCharState*) const;
	void SetState(const BtnCharState*);

private:
	BtnChar(const BtnChar&);
	int GetKeyEventBitmap() const;
	int GetJPhoneMailEventBitmap() const;
	virtual const CharType& GetCharType() const;
	virtual Char* Clone() const;
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual void DrawPreview(const CharPreview*) const;
	virtual void Draw(CDC* pDC,const AffineMatrix*,const Coloring*) const;
	virtual void GetRect(CRect& rc) const;
	virtual void GetBezier(CBEZIER&) const;
	static void SetBit(int& bits,int bit,BOOL on);
};


