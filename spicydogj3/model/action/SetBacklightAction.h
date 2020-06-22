#pragma once

#include "Action.h"

class SetBacklightAction : public Action {
public:
	SetBacklightAction();
	BOOL GetEnabled() const;
	void SetEnabled(BOOL);

protected:
	SetBacklightAction(const SetBacklightAction&);
	virtual ~SetBacklightAction();
	virtual Action* Clone() const;	
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual UINT GetActionNameResID() const;
	virtual CRuntimeClass* GetDialogClass() const;

	BOOL m_enabled;
};