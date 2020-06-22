#pragma once
#include "sdom/OLM.h"
#include "wnd/ViewSwitch.H"
class Char;
class SpicydogDoc;

/** �L�����N�^��ҏW����_�C�A���O */
class CharDialog : public CDialog {
private:
	SpicydogDoc* m_pDocument;
	Char* m_pChar;

public:
	DECLARE_DYNCREATE(CharDialog);
	CharDialog();
	virtual void Init(UINT);
	virtual void AttachChar(SpicydogDoc*,Char* p);

	const Char* GetChar() const;
	Char* GetChar();
	SpicydogDoc* GetDocument();
	const SpicydogDoc* GetDocument() const;

public:
	virtual ~CharDialog();
};

/** �L�����N�^�[�\���r���[�̋K��N���X */
class CharView : public ViewSwitchListener {
protected:
	TOLMReferer<Char> m_pChar;

public:
	virtual void AttachChar(Char* p);

	const Char* GetChar() const;
	Char* GetChar();
	virtual BOOL GetViewCaption(CString& str) const;
public:
	virtual ~CharView();
	virtual void OnActiveViewChanged(int,int);
};

/**	�e�L�����N�^�^�Z�[�t���L�����N�^�[�r���[�N���X */
template <typename _VT>
class TCharView : public CharView {
public:
	const _VT* GetCharT() const {
		const Char* p = GetChar();
		ASSERT_VALID(p);
		return dynamic_cast<const _VT*>(p);
	}

	_VT* GetCharT() {
		Char* p = GetChar();
		ASSERT_VALID(p);
		return dynamic_cast<_VT*>(p); 
	}
};