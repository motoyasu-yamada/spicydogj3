#pragma once

#include "Action.H"

/** �p�����[�^�Ȃ��A�N�V��������N���X */
class SimpleAction : public Action {
private:
	virtual void Build(Builder*) const = 0;
	virtual Action* Clone() const = 0;
	virtual UINT GetActionNameResID() const = 0;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual CRuntimeClass* GetDialogClass() const;
};
