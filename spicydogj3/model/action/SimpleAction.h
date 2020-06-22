#pragma once

#include "Action.H"

/** パラメータなしアクション既定クラス */
class SimpleAction : public Action {
private:
	virtual void Build(Builder*) const = 0;
	virtual Action* Clone() const = 0;
	virtual UINT GetActionNameResID() const = 0;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual CRuntimeClass* GetDialogClass() const;
};
