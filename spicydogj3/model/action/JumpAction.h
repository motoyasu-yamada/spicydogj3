#pragma once

#include "Action.h"
class Label;

class JumpAction : public Action {
public:
	JumpAction();
	const Label* GetLabel() const;
	void SetLabel(const Label*);

protected:
	JumpAction(const JumpAction&);
	virtual ~JumpAction();
	virtual Action* Clone() const;
	virtual void AssertValid() const;
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual UINT GetActionNameResID() const;
	virtual CRuntimeClass* GetDialogClass() const;

	TOLMReferer<Label> m_label;

};