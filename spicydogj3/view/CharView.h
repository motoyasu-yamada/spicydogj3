#pragma once
#include "sdom/OLM.h"
#include "wnd/ViewSwitch.H"
class Char;
class SpicydogDoc;

/** キャラクタを編集するダイアログ */
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

/** キャラクター表示ビューの規定クラス */
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

/**	各キャラクタ型セーフ名キャラクタービュークラス */
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