#pragma once

#include "SimpleAction.h"

class PlayAction: public SimpleAction {
public:
	PlayAction();
	virtual void Build(Builder*) const;
	virtual Action* Clone() const;
	virtual UINT GetActionNameResID() const;
};
