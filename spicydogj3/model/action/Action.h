#pragma once

#include "model/build/Buildable.h"
#include "sdom/olm.h"

class SDOMArchive;

/** アクションの基底クラス */
class Action
:	public COLMObject,
	public Buildable {
	friend class TOLMOwners<Action,false>;
public:
	virtual ~Action();
	virtual Action* Clone() const = 0;
	virtual CRuntimeClass* GetDialogClass() const = 0;
	CString GetActionName() const;
protected:
	Action();
	virtual void Serialize(SDOMArchive*) = 0;
	virtual void AssertValid() const = 0;
	virtual void Build(Builder*) const = 0;
	virtual UINT GetActionNameResID() const = 0;
	static SDOMSerializable* factory(LPCWSTR);
	static LPCWSTR namer(SDOMSerializable*);
};