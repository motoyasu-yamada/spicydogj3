#pragma once

#include "SimpleAction.h"

class SoundStopAction: public SimpleAction {
public:
	SoundStopAction();
	virtual void Build(Builder*) const;
	virtual Action* Clone() const;
	virtual UINT GetActionNameResID() const;
};
