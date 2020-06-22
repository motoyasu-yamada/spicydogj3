#pragma once

#include "SimpleAction.h"

class StopAction: public SimpleAction {
public:
	StopAction();
private:
	virtual void Build(Builder*) const;
	virtual Action* Clone() const;
	virtual UINT GetActionNameResID() const;
};
