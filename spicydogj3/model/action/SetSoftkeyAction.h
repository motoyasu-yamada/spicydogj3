#pragma once

#include "Action.h"

enum SOFTKEY_WHICH {
	SOFTKEY_WHICH_LEFT  = 0,
	SOFTKEY_WHICH_RIGHT = 1
};

/** ソフトキー設定アクション */
class SetSoftkeyAction : public Action {
public:
	SetSoftkeyAction();
	CString GetLabel() const;
	int GetWhich() const;
	void SetSoftkey(int which,LPCTSTR label);

private:
	SetSoftkeyAction(const SetSoftkeyAction&);
	virtual ~SetSoftkeyAction();
	virtual Action* Clone() const;	
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual UINT GetActionNameResID() const;
	virtual CRuntimeClass* GetDialogClass() const;

	int     m_which;
	CString m_label;
};
