#pragma once

#include "Action.h"

class Label;

/** ’x‰„ƒWƒƒƒ“ƒv */
class DelayJumpAction : public Action {
public:
	DelayJumpAction();
	void SetLabel(UINT  delay,const Label*  label);
	UINT GetDelay() const;
	const Label* GetLabel() const;

protected:
	DelayJumpAction(const DelayJumpAction&);
	virtual ~DelayJumpAction();
	virtual Action* Clone() const;
	virtual void Build(Builder*) const;
	virtual void Serialize(SDOMArchive*);
	virtual void AssertValid() const;
	virtual UINT GetActionNameResID() const;
	virtual CRuntimeClass* GetDialogClass() const;
private:
	UINT               m_delay;
	TOLMReferer<Label> m_label;
};
